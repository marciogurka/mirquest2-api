from rest_framework import serializers

from request_records.models import RequestRecord, RequestInfo
from tools.models import Tool
from tools.api.serializers import ToolSerializer

import json
import re
import subprocess
import os 
from pathlib import Path
from datetime import datetime, timezone
from django.core.mail import send_mail
from django.core.mail import EmailMultiAlternatives
from background_task import background
import time
import glob

from django.conf import settings

class RequestRecordSerializer(serializers.ModelSerializer): #forms.ModelForm
    tools = serializers.PrimaryKeyRelatedField(queryset=Tool.objects.all(), many=True)

    class Meta:
        model = RequestRecord
        fields = [
            "pk",
            "userName",
            "userEmail",
            "fileName",
            "status",
            "code",
            "createdDate",
            "endDate",
            "totalTime",
            "tools",
            "file"
        ]

        read_only_fields = ["code", "pk"]

    def create(self, validated_data):
        tools_data = validated_data.pop('tools')
        request_record = RequestRecord.objects.create(**validated_data)
        for tool_id in tools_data:
            request_info = RequestInfo.objects.create(
                request=request_record,
                tool=tool_id,
                requestCode=request_record.code,
                userEmail=request_record.userEmail
            )
        subject, from_email, to = 'Your request has been created!', 'marciogurka@marciogurka.com', request_record.userEmail
        text_content = ''
        html_content = '<p>Your request has been created and is now processing, check the status with the request code!</p><p>Your request code: <strong>' + \
            str(request_record.code) + '</strong></p>'
        msg = EmailMultiAlternatives(subject, text_content, from_email, [to])
        msg.attach_alternative(html_content, "text/html")
        if (settings.ENABLE_EMAIL):
            msg.send()
        return request_record

    def update(self, instance, validated_data):
        instance.userName = validated_data.get('userName', instance.userName)
        instance.userEmail = validated_data.get('userEmail', instance.userEmail)
        instance.fileName = validated_data.get('fileName', instance.fileName)
        instance.status = validated_data.get('status', instance.status)
        instance.endDate = validated_data.get('endDate', instance.endDate)
        instance.totalTime = validated_data.get('totalTime', instance.totalTime)
        instance.file = validated_data.get('file', instance.file)
        if(instance.fileName != instance.get_filename()):
            instance.fileName = instance.get_filename()
        instance.save()
        if(instance.status == "PROCESSING"):
            RequestRecordSerializer.startToolProcessFile(instance.pk)
        return instance

    @background(schedule=10)
    def startToolProcessFile(request_record_id):
        request_record = RequestRecord.objects.get(pk=request_record_id)
        tools = [val for val in request_record.tools.all() if val in Tool.objects.all()]
        for tool in tools:
            # mirinho
            if(tool.pk == 1):
                filepath = request_record.pathname()
                path = Path(filepath).resolve()
                filename, fileFormat = os.path.splitext(request_record.fileName)
                output_file_name = str(path) + "/" + request_record.fileName.split(fileFormat)[0] + "_out.fa"
                input_file_name = str(path) + "/" + request_record.fileName
                command = "cd ~/mirquest2-api/mirinho/ && ./mirinho -o '" + output_file_name + "' -i '" + input_file_name + "'"
                start_time = time.time()
                mirinhoResponse = subprocess.run([command], shell=True, capture_output=True)
                requestTime = (time.time() - start_time)
                if(mirinhoResponse.returncode == 0):
                    # get all request info about that request
                    requests_info = RequestInfo.objects.all().filter(request=request_record)
                    # get the request info about mirinho
                    request_info = next((x for x in requests_info if x.tool == tool), None)
                    if(request_info is not None):
                        request_info.status = "PROCESSED"
                        request_info.endDate = datetime.now(timezone.utc)
                        request_info.totalTime = datetime.utcfromtimestamp(requestTime).time()
                        request_info.save()
                        # sending email
                        subject, from_email, to = 'Your Mirinho request has just finished!', 'marciogurka@marciogurka.com', request_record.userEmail
                        text_content = ''
                        html_content = '<p>Your Mirinho request has just finished processing. Check out the result using the code below!</p><p>Your request code: <strong>' + str(request_info.requestCode) + '</strong></p>'
                        msg = EmailMultiAlternatives(subject, text_content, from_email, [to])
                        msg.attach_alternative(html_content, "text/html")
                        if (settings.ENABLE_EMAIL):
                            msg.send()
                    #if there is no more request info in PROCESSING status, update the request
                    request_info_not_finished = next((x for x in requests_info if x.status == "PROCESSING"), None)
                    if(request_info_not_finished is None):
                        request_record.status = "PROCESSED"
                        request_record.endDate = datetime.now(timezone.utc)
                        total_time = request_record.endDate - request_record.createdDate
                        request_record.totalTime = (datetime.min + total_time).time()
                        request_record.save()
                else:
                    subject, from_email, to = 'Erro na request mirinho!', 'marciogurka@marciogurka.com', 'marciogurkajr@gmail.com'
                    text_content = ''
                    html_content = '<p>Erro na request mirinho - código: <strong>' + \
                        str(request_record.code) + '</strong></p>' + \
                        '<p>stdout: ' + str(mirinhoResponse.stdout) + '</p>' + \
                        '<p>stderr: ' + str(mirinhoResponse.stderr) + '</p>'
                    msg = EmailMultiAlternatives(
                        subject, text_content, from_email, [to])
                    msg.attach_alternative(html_content, "text/html")
                    if (settings.ENABLE_EMAIL):
                        msg.send()
            elif(tool.id == 2): 
                # set file names
                filepath = request_record.pathname()
                path = Path(filepath).resolve()
                filename, fileFormat = os.path.splitext(request_record.fileName)
                processed_file_name = str(path) + "/" + request_record.fileName.split(fileFormat)[0] + "_processed" + fileFormat
                input_file_name = str(path) + "/" + request_record.fileName
                example_pos_fileName = str(path) + "/" + request_record.fileName.split(fileFormat)[0] + "_pos.txt"
                example_neg_fileName = str(path) + "/"+ request_record.fileName.split(fileFormat)[0] + "_neg.txt"
                example_pos_fileName_with_path = str(path) + "/" + request_record.fileName.split(fileFormat)[0] + "_pos.txt"
                example_neg_fileName_with_path = str(path) + "/" + request_record.fileName.split(fileFormat)[0] + "_neg.txt"
                example_train_fileName = str(path) + "/" + request_record.fileName.split(fileFormat)[0] + "_train.txt"
                example_train_extracted_fileName = str(path) + "/" + request_record.fileName.split(fileFormat)[0] + "_train_extracted.txt"
                example_test_fileName = str(path) + "/" + request_record.fileName.split(fileFormat)[0] + "_test.txt"
                example_test_extracted_fileName = str(path) + "/" + request_record.fileName.split(fileFormat)[0] + "_test_extracted.txt"
                # process FASTA file to avoid segmentation fault
                RequestRecordSerializer.processMirboostFile(request_record)
                # build commands
                command0 = "cd ~/mirquest2-api/miRBoost/"
                command1 = "./gFoldmulti/gFold -s '" + processed_file_name + "' -L 450 -DATA 1 > '" + example_pos_fileName + "'"
                command2 = "./gFoldmulti/gFold -s '" + processed_file_name + "' -L 450 -DATA -1 > '" + example_neg_fileName + "'"
                command3 = "cat '" + example_pos_fileName_with_path + "' '" + example_neg_fileName_with_path + "' > '" + example_train_fileName + "'"
                command4 = "head -100 '" + example_pos_fileName_with_path + "' > '" + example_test_fileName + "'"
                command5 = "head -100 '" + example_neg_fileName_with_path + "' >> '" + example_test_fileName + "'"
                # cmds for humans features
                command6 = "./src/extractfeature.pl -s human_selected_features.txt -i '" + example_test_fileName + "' -o '" + example_train_extracted_fileName + "'"
                command7 = "./src/extractfeature.pl -s human_selected_features.txt -i '" + example_test_fileName + "' -o '" + example_test_extracted_fileName + "'"
                command8 = "./miRBoost -i '" + example_test_extracted_fileName + "' -t '" + example_train_extracted_fileName + "' -d 0.25"
                fullComand = command0 + " ; " + command1 + " ; " + command2 + " ; " + command3 + " ; " + command4 + " ; " + command5 + " ; " + command6 + " ; " + command7 + " ; " + command8 + " ; "
                mirboostHumanResponse = subprocess.run([fullComand], shell=True, capture_output=True)
                # cmds for whole features
                # command6 = "./src/extractfeature.pl -s whole_selected_features.txt -i '" + example_test_fileName + "' -o '" + example_train_extracted_fileName + "'"
                # command7 = "./src/extractfeature.pl -s whole_selected_features.txt -i '" + example_test_fileName + "' -o '" + example_test_extracted_fileName + "'"
                # command8 = "./miRBoost -i '" + example_test_extracted_fileName + "' -t '" + example_train_extracted_fileName + "' -d 0.25"
                # fullComand = command1 + " ; " + command2 + " ; " + command3 + " ; " + command4 + " ; " + command5 + " ; " + command6 + " ; " + command7 + " ; " + command8 + " ; "
                # mirboostWholeResponse = subprocess.run([fullComand], shell=True, capture_output=True)
                if(mirboostHumanResponse.returncode == 0):
                    # move result file 
                    pattern = "results*.txt"
                    matches = re.findall(pattern, str(mirboostHumanResponse.stdout.decode('utf-8')))
                    if(matches): 
                        resultFileName = str(matches[0])
                        command0 = "cd ~/mirquest2-api/miRBoost/"
                        rnCommand = "mv '" + resultFileName + "' result_mirboost_" + str(request_record.code) + ".txt"
                        mvCommand = "mv result_mirboost_" + str(request_record.code) + ".txt " + str(path) + ""
                        mvCommandResponse = subprocess.run([command0 + ";" + rnCommand + ";" + mvCommand], shell=True, capture_output=True)
                        if(mvCommandResponse.returncode == 0): 
                            # get all request info about that request
                            requests_info = RequestInfo.objects.all().filter(request=request_record)
                            # get the request info about mirboost
                            request_info = next((x for x in requests_info if x.tool == tool), None)
                            if(request_info is not None):
                                request_info.status = "PROCESSED"
                                request_info.endDate = datetime.now(timezone.utc)
                                total_time = request_info.endDate - request_info.createdDate
                                request_info.totalTime = (datetime.min + total_time).time()
                                request_info.save()
                                # sending email
                                subject, from_email, to = 'Your miRBoost request has just finished!', 'marciogurka@marciogurka.com', request_record.userEmail
                                text_content = ''
                                html_content = '<p>Your miRBoost request has just finished processing. Check out the result using the code below!</p><p>Your request code: <strong>' + str(request_info.requestCode) + '</strong></p>'
                                msg = EmailMultiAlternatives(subject, text_content, from_email, [to])
                                msg.attach_alternative(html_content, "text/html")
                                if (settings.ENABLE_EMAIL):
                                    msg.send()
                            #if there is no more request info in PROCESSING status, update the request
                            request_info_not_finished = next((x for x in requests_info if x.status == "PROCESSING"), None)
                            if(request_info_not_finished is None):
                                request_record.status = "PROCESSED"
                                request_record.endDate = datetime.now(timezone.utc)
                                total_time = request_record.endDate - request_record.createdDate
                                request_record.totalTime = (datetime.min + total_time).time()
                                request_record.save()
                        else: 
                            subject, from_email, to = 'Erro na request mirboost!', 'marciogurka@marciogurka.com', 'marciogurkajr@gmail.com'
                            text_content = ''
                            html_content = '<p>Erro na request mirboost - código: <strong>' + \
                                str(request_record.code) + '</strong></p>' + \
                                '<p>stdout: ' + str(mirboostHumanResponse.stdout) + '</p>' + \
                                '<p>stderr: ' + str(mirboostHumanResponse.stderr) + '</p>'
                            msg = EmailMultiAlternatives(
                                subject, text_content, from_email, [to])
                            msg.attach_alternative(html_content, "text/html")
                            if (settings.ENABLE_EMAIL):
                                msg.send()
                    else: 
                        pattern = "miRBoost/results*.txt"
                        resultFileArray = glob.glob(pattern)
                        if(resultFileArray): 
                            resultFileName = str(resultFileArray[0])
                            rnCommand = "mv '" + resultFileName + "' result_mirboost_" + str(request_record.code) + ".txt"
                            mvCommand = "mv result_mirboost_" + str(request_record.code) + ".txt " + str(path) + ""
                            mvCommandResponse = subprocess.run([rnCommand + ";" + mvCommand], shell=True, capture_output=True)
                            if(mvCommandResponse.returncode == 0): 
                                # get all request info about that request
                                requests_info = RequestInfo.objects.all().filter(request=request_record)
                                # get the request info about mirboost
                                request_info = next((x for x in requests_info if x.tool == tool), None)
                                if(request_info is not None):
                                    request_info.status = "PROCESSED"
                                    request_info.endDate = datetime.now(timezone.utc)
                                    total_time = request_info.endDate - request_info.createdDate
                                    request_info.totalTime = (datetime.min + total_time).time()
                                    request_info.save()
                                    # sending email
                                    subject, from_email, to = 'Your miRBoost request has just finished!', 'marciogurka@marciogurka.com', request_record.userEmail
                                    text_content = ''
                                    html_content = '<p>Your miRBoost request has just finished processing. Check out the result using the code below!</p><p>Your request code: <strong>' + str(request_info.requestCode) + '</strong></p>'
                                    msg = EmailMultiAlternatives(subject, text_content, from_email, [to])
                                    msg.attach_alternative(html_content, "text/html")
                                    if (settings.ENABLE_EMAIL):
                                        msg.send()
                                #if there is no more request info in PROCESSING status, update the request
                                request_info_not_finished = next((x for x in requests_info if x.status == "PROCESSING"), None)
                                if(request_info_not_finished is None):
                                    request_record.status = "PROCESSED"
                                    request_record.endDate = datetime.now(timezone.utc)
                                    total_time = request_record.endDate - request_record.createdDate
                                    request_record.totalTime = (datetime.min + total_time).time()
                                    request_record.save()
                            else: 
                                subject, from_email, to = 'Erro na request mirboost!', 'marciogurka@marciogurka.com', 'marciogurkajr@gmail.com'
                                text_content = ''
                                html_content = '<p>Erro na request mirboost - código: <strong>' + \
                                    str(request_record.code) + '</strong></p>' + \
                                    '<p>stdout: ' + str(mirboostHumanResponse.stdout) + '</p>' + \
                                    '<p>stderr: ' + str(mirboostHumanResponse.stderr) + '</p>'
                                msg = EmailMultiAlternatives(
                                    subject, text_content, from_email, [to])
                                msg.attach_alternative(html_content, "text/html")
                                if (settings.ENABLE_EMAIL):
                                    msg.send()
                        else:
                            # get all request info about that request
                            requests_info = RequestInfo.objects.all().filter(request=request_record)
                            # get the request info about mirboost
                            request_info = next((x for x in requests_info if x.tool == tool), None)
                            if(request_info is not None):
                                request_info.status = "PROCESSED"
                                request_info.endDate = datetime.now(timezone.utc)
                                total_time = request_info.endDate - request_info.createdDate
                                request_info.totalTime = (datetime.min + total_time).time()
                                request_info.save()
                                # sending email
                                subject, from_email, to = 'Your miRBoost request has just finished!', 'marciogurka@marciogurka.com', request_record.userEmail
                                text_content = ''
                                html_content = '<p>Your miRBoost request has just finished processing. Check out the result using the code below!</p><p>Your request code: <strong>' + str(request_info.requestCode) + '</strong></p>'
                                msg = EmailMultiAlternatives(subject, text_content, from_email, [to])
                                msg.attach_alternative(html_content, "text/html")
                                if (settings.ENABLE_EMAIL):
                                    msg.send()
                            #if there is no more request info in PROCESSING status, update the request
                            request_info_not_finished = next((x for x in requests_info if x.status == "PROCESSING"), None)
                            if(request_info_not_finished is None):
                                request_record.status = "PROCESSED"
                                request_record.endDate = datetime.now(timezone.utc)
                                total_time = request_record.endDate - request_record.createdDate
                                request_record.totalTime = (datetime.min + total_time).time()
                                request_record.save()
                else: 
                    subject, from_email, to = 'Erro na request mirboost!', 'marciogurka@marciogurka.com', 'marciogurkajr@gmail.com'
                    text_content = ''
                    html_content = '<p>Erro na request mirboost - código: <strong>' + \
                        str(request_record.code) + '</strong></p>' + \
                        '<p>stdout: ' + str(mirboostHumanResponse.stdout) + '</p>' + \
                        '<p>stderr: ' + str(mirboostHumanResponse.stderr) + '</p>'
                    msg = EmailMultiAlternatives(
                        subject, text_content, from_email, [to])
                    msg.attach_alternative(html_content, "text/html")
                    if (settings.ENABLE_EMAIL):
                        msg.send()
    
    def processMirboostFile(request_record): 
        filepath = request_record.pathname()
        path = Path(filepath).resolve()
        input_file_name = str(path) + "/" + request_record.fileName
        filename, fileFormat = os.path.splitext(request_record.fileName)
        output_file_name = str(path) + "/" + request_record.fileName.split(fileFormat)[0] + "_processed" + fileFormat
        my_file = open(input_file_name, 'r')
        lines = my_file.readlines()
        y = ''
        fileText = ""
        text = []
        for line in lines:
            if line.rstrip("\r\n"):
                if line.rstrip("\r\n").find(">") == 0:
                    if text:
                        fileText = fileText + ''.join(text)
                        fileText = fileText + "\n"
                    fileText = fileText + line.rstrip("\r\n")
                    fileText = fileText + "\n"
                    y = ''
                    text = []
                else:
                    y = ''.join(line.rstrip("\r\n"))
                    text.append(y)
        fileText = fileText + ''.join(text)

        fh = open(output_file_name, "w")
        fh.write(fileText)
        fh.close()

class RequestInfoSerializer(serializers.ModelSerializer):  # forms.ModelForm
    tool = ToolSerializer(many=False, read_only=True)
    request = RequestRecordSerializer(many=False, read_only=True)

    class Meta:
        model = RequestInfo
        fields = [
            "pk",
            "request",
            "tool",
            "userEmail",
            "requestCode",
            "createdDate",
            "endDate",
            "totalTime",
            "status",
        ]

        read_only_fields = ["pk", "createdDate"]
    

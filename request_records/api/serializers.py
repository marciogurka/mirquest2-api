from rest_framework import serializers

from request_records.models import RequestRecord, RequestInfo
from tools.models import Tool
from tools.api.serializers import ToolSerializer

import json
import subprocess
from pathlib import Path
from datetime import datetime, timezone
from django.core.mail import send_mail
from django.core.mail import EmailMultiAlternatives

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
            self.startToolProcessFile(instance)
        return instance

    def startToolProcessFile(self, request_record):
        tools = [val for val in request_record.tools.all() if val in Tool.objects.all()]
        for tool in tools:
            # mirinho
            if(tool.pk == 1):
                filepath = request_record.pathname()
                path = Path(filepath).resolve()
                output_file_name = str(path) + "/" + request_record.fileName.split(".fa")[0] + "_out.fa"
                input_file_name = str(path) + "/" + request_record.fileName
                command = "cd mirinho/ && ./mirinho -s=0 -o '" + output_file_name + "' -i '" + input_file_name + "'"
                mirinhoResponse = subprocess.run([command], shell=True, capture_output=True)
                if(mirinhoResponse.returncode == 0):
                    # get all request info about that request
                    requests_info = RequestInfo.objects.all().filter(request=request_record)
                    # get the request info about mirinho
                    request_info = next((x for x in requests_info if x.tool == tool), None)
                    if(request_info != None):
                        request_info.status = "PROCESSED"
                        request_info.endDate = datetime.now(timezone.utc)
                        total_time = request_info.endDate - request_info.createdDate
                        request_info.totalTime = (datetime.min + total_time).time()
                        request_info.save()
                        # sending email
                        subject, from_email, to = 'Your Mirinho request has just finished!', 'marciogurka@marciogurka.com', request_record.userEmail
                        text_content = ''
                        html_content = '<p>Your Mirinho request has just finished processing. Check out the result using the code below!</p><p>Your request code: <strong>' + str(request_info.requestCode) + '</strong></p>'
                        msg = EmailMultiAlternatives(subject, text_content, from_email, [to])
                        msg.attach_alternative(html_content, "text/html")
                        msg.send()
                    #if there is no more request info in PROCESSING status, update the request
                    request_info_not_finished = next((x for x in requests_info if x.status == "PROCESSING"), None)
                    if(request_info_not_finished == None):
                        request_record.status = "PROCESSED"
                        request_record.endDate = datetime.now(timezone.utc)
                        total_time = request_record.endDate - request_record.createdDate
                        request_record.totalTime = (datetime.min + total_time).time()
                        request_record.save()
                else:
                    subject, from_email, to = 'Your Mirinho request has just finished!', 'marciogurka@marciogurka.com', 'marciogurkajr@gmail.com'
                    text_content = ''
                    html_content = '<p>Erro na request mirinho - código: <strong>' + \
                        str(request_record.code) + '</strong></p>' + \
                        '<p>stdout: ' + str(mirinhoResponse.stdout) + '</p>' + \
                        '<p>stderr: ' + str(mirinhoResponse.stderr) + '</p>'
                    msg = EmailMultiAlternatives(
                        subject, text_content, from_email, [to])
                    msg.attach_alternative(html_content, "text/html")
                    msg.send()

class RequestInfoSerializer(serializers.ModelSerializer):  # forms.ModelForm
    tool = ToolSerializer(many=False, read_only=True)

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
    

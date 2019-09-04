from django.db import models
from datetime import datetime
from uuid import uuid4
from tools.models import Tool
import os


def get_upload_path(instance, filename):
    return os.path.join("files", instance.code, filename)

class RequestRecord(models.Model):
    # pk aka id --> number
    userName = models.CharField(max_length=100, blank=True)
    userEmail = models.CharField(max_length=100, blank=False, null=False)
    fileName = models.CharField(max_length=100, blank=True)
    status = models.CharField(max_length=50, default="PROCESSING")
    code = models.CharField(max_length=80, default=uuid4, unique=True)
    createdDate = models.DateTimeField(auto_now_add=True, blank=True)
    endDate = models.DateTimeField(default=None, blank=True, null=True)
    totalTime = models.TimeField(default=None, blank=True, null=True)
    tools = models.ManyToManyField(Tool, through="RequestInfo")
    file = models.FileField(default=None, null=True, upload_to=get_upload_path) 

    def get_filename(self):
        return os.path.basename(self.file.name)
    
    def pathname(self):
        return os.path.dirname(self.file.name)


class RequestInfo(models.Model):
    request = models.ForeignKey(RequestRecord, on_delete=models.CASCADE)
    tool = models.ForeignKey(Tool, on_delete=models.CASCADE)
    requestCode = models.CharField(max_length=80)
    userEmail = models.CharField(max_length=100, blank=True, null=False)
    createdDate = models.DateTimeField(auto_now_add=True, blank=True)
    endDate = models.DateTimeField(default=None, blank=True, null=True)
    totalTime = models.TimeField(default=None, blank=True, null=True)
    status = models.CharField(max_length=50, default="PROCESSING")

from django.contrib import admin

# Register your models here.
from .models import RequestRecord

admin.site.register(RequestRecord)

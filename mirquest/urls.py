"""mirquest URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.conf.urls import url, include
from django.contrib import admin

urlpatterns = [
    url('admin/', admin.site.urls),
    # url(r'^api/auth/login/$', obtain_jwt_token, name='api-login'),
    url(r'^api/request_records/', include(('request_records.api.urls', 'api-request-records'), namespace='api-request-records')),
    url(r'^api/tools/', include(('tools.api.urls', 'api-tools'), namespace='api-tools')),
]

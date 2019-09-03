from django.conf.urls import url, include

from .views import ToolRudView, ToolAPIView

urlpatterns = [
    url(r'^$', ToolAPIView.as_view(), name='tool-create'),
    url(r'^(?P<pk>\d+)/$', ToolRudView.as_view(), name='tool-rud')
]

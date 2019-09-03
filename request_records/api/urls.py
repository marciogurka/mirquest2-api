from django.conf.urls import url, include

from .views import RequestRecordRudView, RequestRecordAPIView, RequestInfoAPIView

urlpatterns = [
    url(r'^$', RequestRecordAPIView.as_view(), name='request-record-create'),
    url(r'^(?P<pk>\d+)/$', RequestRecordRudView.as_view(), name='request-record-rud'),
    url(r'^request_info/$', RequestInfoAPIView.as_view(), name='request-info-rud')
]

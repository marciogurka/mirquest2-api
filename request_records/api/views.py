from django.db.models import Q
from rest_framework import generics, mixins

from request_records.models import RequestRecord, RequestInfo
from request_records.api.serializers import RequestRecordSerializer, RequestInfoSerializer

class RequestRecordAPIView(mixins.CreateModelMixin, generics.ListAPIView):
    lookup_field = 'pk'
    serializer_class = RequestRecordSerializer
    #queryset        = RequestRecord.objects.all()

    def get_queryset(self):
        qs = RequestRecord.objects.all()
        query = self.request.GET.get("q")
        # filter by code or email
        if query is not None: 
            qs = qs.filter(Q(code__icontains=query) | Q(userEmail__icontains=query))
        return qs
    
    def post(self, request, *args, **kwargs):
        return self.create(request, *args, **kwargs)

class RequestRecordRudView(generics.RetrieveUpdateDestroyAPIView): #DetailView CreateView FormView
    lookup_field    = 'pk'
    serializer_class = RequestRecordSerializer
    #queryset        = RequestRecord.objects.all()

    def get_queryset(self):
        return RequestRecord.objects.all()

    # def get_object(self):
    #     pk = self.kwargs.get("pk")
    #     return RequestRecord.objects.get(pk=pk)


class RequestInfoAPIView(mixins.CreateModelMixin, generics.ListAPIView):
    lookup_field = 'pk'
    serializer_class = RequestInfoSerializer
    queryset = RequestInfo.objects.all()

    def get_queryset(self):
        qs = RequestInfo.objects.all()
        query = self.request.GET.get("q")
        # filter by code or email
        if query is not None:
            qs = qs.filter(Q(userEmail__icontains=query) |
                           Q(requestCode__icontains=query))
        return qs


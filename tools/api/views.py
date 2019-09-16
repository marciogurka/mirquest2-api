from django.db.models import Q
from rest_framework import generics, mixins

from tools.models import Tool
from tools.api.serializers import ToolSerializer


class ToolAPIView(mixins.CreateModelMixin, generics.ListAPIView):
    lookup_field = 'pk'
    serializer_class = ToolSerializer
    #queryset        = RequestRecord.objects.all()

    def get_queryset(self):
        qs = Tool.objects.all()
        query = self.request.GET.get("q")
        # filter by code or email
        if query is not None:
            qs = qs.filter(Q(code__icontains=query) |
                           Q(userEmail__icontains=query))
        return qs

    def post(self, request, *args, **kwargs):
        return self.create(request, *args, **kwargs)


# DetailView CreateView FormView
class ToolRudView(generics.RetrieveUpdateDestroyAPIView):
    lookup_field = 'pk'
    serializer_class = ToolSerializer
    #queryset        = RequestRecord.objects.all()

    def get_queryset(self):
        return Tool.objects.all()

    # def get_object(self):
    #     pk = self.kwargs.get("pk")
    #     return RequestRecord.objects.get(pk=pk)

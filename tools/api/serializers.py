from rest_framework import serializers

from tools.models import Tool


class ToolSerializer(serializers.ModelSerializer):  # forms.ModelForm
    class Meta:
        model = Tool
        fields = [
            "pk",
            "name",
            "createdDate",
        ]

        read_only_fields = ["pk"]

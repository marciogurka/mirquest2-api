from django.db import models

class Tool(models.Model):
    # pk aka id --> number
    name = models.CharField(max_length=100, blank=True)
    createdDate = models.DateTimeField(auto_now_add=True, blank=True)
    def __str__(self):
        return self.name

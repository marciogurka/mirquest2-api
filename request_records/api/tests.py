from rest_framework.test import APITestCase

from django.contrib.auth import get_user_model

from request_records.models import RequestRecord
User = get_user_model()

class RequestRecordAPITestCase(APITestCase):
    def setUp(self):
        user_obj = User(username="marcio", email="test@test.com")
        user_obj.set_password("password")
        user_obj.save()
        request_record = RequestRecord.objects.create(
            userName="user",
            userEmail="user@email.com",
            fileName="arquivo",
            code=123,
            isMirboost=0,
            isMirinho=1
        )
    
    def test_single_user(self):
        user_count = User.objects.count()
        self.assertEqual(user_count, 1)
        
    def test_single_request(self):
        request_count = RequestRecord.objects.count()
        self.assertEqual(request_count, 1)

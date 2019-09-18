"""
WSGI config for mirquest project.

It exposes the WSGI callable as a module-level variable named ``application``.

For more information on this file, see
https://docs.djangoproject.com/en/2.1/howto/deployment/wsgi/
"""

import os

from django.core.wsgi import get_wsgi_application

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'mirquest.settings')

# add the project path into the sys.path
os.path.append('/var/www/html/mirquest2/mirquest2-api')
# add the virtualenv site-packages path to the sys.path
os.path.append('/var/www/html/mirquest2/mirquest2-api/lib/python3.7/site-packages')

application = get_wsgi_application()

TEMPLATE = aux

desktop.path = /usr/share/applications
desktop.files = cookie-test.desktop
INSTALLS += desktop

application.path = /usr/share/accounts/applications
application.files = cookie-test.application
INSTALLS += application

services.path = /usr/share/accounts/services
services.files = \
    google-webapp.service \
    facebook-webapp.service
INSTALLS += services

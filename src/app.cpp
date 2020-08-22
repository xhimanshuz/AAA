#include <QApplication>

#include "ReleaseOrder/AddReleaseOrder.h"
#include "UserUI/UserWindow.h"
#include "JobType.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    NewUser newUser("Anthem Advertising Agency", QStringList()<<"Dalip Rastogi"<<"9058613131"<<"dalip9837056007@gmail.com"<<"B14,\nGandhi Nagar"
                    << "Moradabad" << "Uttar Pradesh"<< "ABLK1239001"<<"244001", USER_TYPE::CLIENT);

    newUser.show();

    NewUser newMedia("SIRMAUR SOAP", QStringList()<<"Sardar Ji"<<"9302832032"<<"sirmaursoap@gmail.com"<<"A123, Bareilly Road"
                    << "Bareilly" << "Uttar Pradesh"<< "KABL2132130"<<"244004", USER_TYPE::MEDIA_HOUSE);
    newMedia.show();

    AddReleaseOrder ro;
    ro.show();

    JobType jobType;
    jobType.show();

    return app.exec();
}

#include <QString>
#include <cstdio>
#include "stacer-core_global.h"
#include "Types/Applications/desktop.h"
#include "Types/Applications/broken_app.h"
#include "Utils/file_util.h"

using namespace Types::Applications;

int main(int argc, char **argv)
{
    long long total = 0;
    Desktop *desk = nullptr;
    QStringList qsl = FileUtil::getDesktopFiles(&total, true);
   
    int broke_nope  = 0;
    int unbroke_yup = 0; 
    for(auto a = qsl.begin(); a != qsl.end(); ++a)
    {
            const auto& c = *a;
	    if (desk != nullptr)
		    delete desk;

	    printf("%s\n", c.toStdString().c_str());
            bool b = loadAndCheckDesktopFile(*a);

	    loadDesktopFile(*a, &desk); 
	    printf("N: %s\nE: %s\nIP: %s\nFP: %s\n",
		desk->second->display_name.toStdString().c_str(),
		desk->second->exec_name.toStdString().c_str(),
		desk->second->install_prefix.toStdString().c_str(),
		desk->second->file_path.toStdString().c_str());

	    if (b)
            {
                printf("broken app\n\n");
                broke_nope++;
		continue;
            }
            printf("unbroken app\n\n");
	    unbroke_yup++;
    }
    if ((broke_nope >= (int)total && unbroke_yup == 0) || unbroke_yup == 0)
	   return 1; 
    
    return 0;
}


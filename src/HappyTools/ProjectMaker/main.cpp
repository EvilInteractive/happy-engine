
#include <string>
#include <iostream>
#include <fstream>

#include <conio.h>

using namespace std;

void findAndReplace(const std::string& projectName, const std::string& file)
{
    fstream inputStream;
    inputStream.open(file + ".tmp", std::ios_base::in);

    fstream outputStream(file);
    outputStream.open(file, std::ios_base::out | ios_base::trunc);

    while (inputStream.eof() == false)
    {
        string line;
        getline(inputStream, line);

        std::string toFind("HappyTemplateProject");
        unsigned int index(line.find(toFind));
        if (index != string::npos)
        {
            std::cout << "Found replace thing \n";
            outputStream << line.substr(0, index);
            outputStream << projectName;
            outputStream << line.substr(index + toFind.size()) << std::endl;
        }
        else
            outputStream << line << std::endl;
    }
    outputStream.flush();

    inputStream.close();
    outputStream.close();


    std::string deleteCommand("del \"" + file + ".tmp\"");
    system(deleteCommand.c_str());
}

int main()
{
    string projectName;

    cout << "-------------------------------------------\n";
    cout << "-------Happy Project Maker-----------------\n";
    cout << "-------------------------------------------\n";
    cout << "\n";
    
    cout << "exit with -1\n\n";
    cout << "Project name: ";
    cin >> projectName;

    if (projectName == "-1")
        return 0;

    cout << "Make project with name: '" << projectName << "'?  ";
    bool cont((char)getch() == 'y');
    if (cont)
    {
        cout << "yes\n";
        cout << "Project is in the make... \n";

        std::string command("CopyProjectFiles.bat " + projectName);
        system(command.c_str());
        
        findAndReplace(projectName, "build\\" + projectName + "\\" + projectName + ".vcxproj");
        findAndReplace(projectName, "build\\" + projectName + "\\" + projectName + ".vcxproj.filters");

        cout << "Project making done! \n";
    }
    else
    {
        cout << "no\n";
    }

    cout << "\n\nPress any key to quit\n";
    getch();
    return 0;
}
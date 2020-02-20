#include <QCoreApplication>

#include <string>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

using namespace boost;
using namespace std;
using namespace boost::filesystem;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc < 3)
    {
        cout << " to little arguments";
        return 0;
    }

    int classDimensionToKeep = stoi(argv[1]);

    string CurrentFileName = argv[2];
    path FileToOpen(CurrentFileName);

    if(!exists(FileToOpen))
    {
        cout << FileToOpen.string() <<" does not exists \n";
        return 0;
    }

    std::ifstream inFile(FileToOpen.string());
    if (!inFile.is_open())
    {
        cout << FileToOpen.string() <<" can notbe opened \n";
        return 0;
    }

    std::vector<std::string> InTextVect;

    string Line;
    while (inFile.good())
    {
        getline(inFile, Line);
        InTextVect.push_back(Line);
    }
    inFile.close();

    std::vector<std::string> OutTextVect;

    //LeaveoneOneDimensonOfClassyfier(3);

    if(InTextVect.empty())
    {
        cout << FileToOpen.string() <<" is empty \n";
        return 0;
    }

    vector<string>::iterator iInTextVect = InTextVect.begin();
    //regex LinePattern;
    string TempStr;
    string NrBuff;

    int newClassifierCount = 0;

    //check First line
    if (!regex_match(*iInTextVect, regex("MzLinearClassifiers.+")) || iInTextVect == InTextVect.end())
    {
        cout << FileToOpen.string() <<" Improper file format first line should begin with \"MzLinearClassifiers\" \n";
        return 0;
    }
    OutTextVect.push_back(*iInTextVect);
    iInTextVect++;

    //ceck second line
    if (!regex_match(*iInTextVect, regex("@Classifiers.+")) || iInTextVect == InTextVect.end())
    {
        cout << FileToOpen.string() <<" Improper file format second line should begin with \"@Classifiers\" \n";
        return a.exec();
    }
    TempStr = *iInTextVect;
    size_t spacePosition = TempStr.find(" ");
    NrBuff = TempStr.substr(spacePosition);
    int classifierCount = stoi(NrBuff);

    OutTextVect.push_back("@Classifiers ");
    iInTextVect++;

    while(iInTextVect != InTextVect.end())
    {
        string Str1 = *iInTextVect;
        iInTextVect++;
        if(iInTextVect == InTextVect.end())
            break;
        string Str2 = *iInTextVect;
        iInTextVect++;
        if(iInTextVect == InTextVect.end())
            break;
        string Str3 = *iInTextVect;
        iInTextVect++;
        if(iInTextVect == InTextVect.end())
            break;
        string Str4 = *iInTextVect;
        iInTextVect++;
        if(iInTextVect == InTextVect.end())
            break;
        string Str5 = *iInTextVect;
        iInTextVect++;
        if(iInTextVect == InTextVect.end())
            break;

        if (!regex_match(Str1, regex("@ClassNames.+")))
            break;

        if (!regex_match(Str2, regex("@FeatureNames.+")))
        {
            OutTextVect.pop_back();
            break;
        }
        size_t firstSpacePosition = Str2.find(" ");
        NrBuff = Str2.substr(firstSpacePosition+1);
        size_t secondSpacePosition = NrBuff.find(" ");
        NrBuff = NrBuff.substr(0,secondSpacePosition);
        int featureCount = stoi(NrBuff);
        if (featureCount != classDimensionToKeep)
            continue;
        OutTextVect.push_back(Str1);
        OutTextVect.push_back(Str2);
        OutTextVect.push_back(Str3);
        OutTextVect.push_back(Str4);
        OutTextVect.push_back(Str5);
        newClassifierCount++;
    }
    OutTextVect[1] += to_string(newClassifierCount);
    string strOut = FileToOpen.stem().string() + " ClassIn#: " + to_string(classifierCount) + " ClassOut#: " + to_string(newClassifierCount) + "\n";
    cout << strOut;

    string TextOut = "";
    for(unsigned int i = 0; i < OutTextVect.size();i++)
        TextOut += OutTextVect[i] + "\n";

    std::ofstream FileToSave(FileToOpen.string());
    if (!FileToSave.is_open())
    {
        cout << "File Not Saved";
        return 0;
    }
    FileToSave << TextOut;
    FileToSave.close();

    cout << "File Saved";

    return 0;
}

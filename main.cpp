#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <cstring>
#include <vector>
#include <getopt.h>
#include <sys/stat.h>
#include <csignal>
#include <filesystem>
#include "search.h"
#include "pattern.h"

using namespace std;

void getFiles(const string &root, vector<string> &files);
int32_t createDirectory(const std::string &directoryPath);


#define ACCESS(fileName,accessMode) access(fileName,accessMode)
#define MKDIR(path) mkdir(path,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)

/**
 * Print logo
 *
 */
void PrintLogo(void)
{
    cout<< R"(
        ________          _   __     __     _____                      __
       / ____/ /__  _  __/ | / /__  / /_   / ___/___  ____ ___________/ /_
      / /_  / / _ \| |/_/  |/ / _ \/ __/   \__ \/ _ \/ __ `/ ___/ ___/ __ \
     / __/ / /  __/>  </ /|  /  __/ /_    ___/ /  __/ /_/ / /  / /__/ / / /
    /_/   /_/\___/_/|_/_/ |_/\___/\__/   /____/\___/\__,_/_/   \___/_/ /_/
                                                                by WillyTerra
    )"<<endl;
}

/**
 * Print help
 *
 */
void PrintUsage(void)
{
    cout <<R"(
    **********************************************************************************
    *  -s        Search flexnet licensing file
    *  -c        Copy flexnet licensing file to new folder
    *  -h        Display this info
    **********************************************************************************
    )"<<endl;
}

/**
 * Main function
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {

    int opt;
    bool flgCopy = false;
    bool flgSearch = false;

    uint16_t PatchSig = 0;

    string pCopyDirectory = "";

    PrintLogo();
    SearchPatternInit();

    while ((opt = getopt(argc, argv, "sc:h")) != -1) {
        switch (opt) {
            case 'c': //Search Copy
                flgCopy = true;
                pCopyDirectory = optarg;
                break;
            case 's': //Search signature
                flgSearch = true;
                break;
            case 'h': //Help
                PrintUsage();
                exit(EXIT_SUCCESS);
                break;
            default: break;
        }
    }

    string root = "."; //Current Directory
    vector<string> files;
    getFiles(root, files);

    for (auto file : files) {

        ifstream binRead(file,ios::binary);
        binRead.seekg(0,ios::end);

        uint32_t dataFileSize = binRead.tellg();
        uint8_t *dataBuffer = new uint8_t [dataFileSize + 10];

        //Read ELF file
        binRead.seekg(0,ios::beg);
        binRead.read((char *)dataBuffer,dataFileSize);
        binRead.close();

        int resData = 0;

        if (flgSearch)
        {
            //Find ECC Pattern
            resData = PatchPatternSearchInfo(dataBuffer,dataFileSize,fnSigPattern);

            if (resData != -1)
            {
                PatchSig++;
                cout<<file<<endl;
                while (dataBuffer[resData + 5] != 0x00)
                {
                    cout<<(char)dataBuffer[resData+5];
                    resData++;
                }
                cout<<endl;

                if (flgCopy) //Copy file to another
                {
                    vector <string> fields;

                    string origFolder = get_current_dir_name() + file.erase(0,1);
                    string copyFolder = pCopyDirectory + file.erase(0,1);

                    createDirectory(copyFolder);

                    std::ifstream  src(origFolder,   std::ios::binary);
                    std::ofstream  dst(copyFolder,   std::ios::binary);
                    dst << src.rdbuf();
                }
            }
        }
        delete []dataBuffer;
    }

    if(flgSearch)
        cout<<"Total "<<PatchSig<<" files with FlexNet license."<< endl;

    return 0;
}

/**
 * Search binary file
 * @param root
 * @param files
 */
void getFiles(const string &root, vector<string> &files) {

    DIR *pDir;
    struct dirent *ptr;

    if (!(pDir = opendir(root.c_str()))) {
        return;
    }

    while ((ptr = readdir(pDir)) != nullptr) {

        string sub_file = root + "/" + ptr->d_name;

        if (ptr->d_type != DT_REG && ptr->d_type != DT_DIR && ptr->d_type != DT_LNK) {
            return;
        }

        if (ptr->d_type == DT_REG) {
            if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {

                uint8_t *dataBuffer = new uint8_t [4];

                ifstream binRead(sub_file,ios::binary);
                binRead.seekg(0,ios::beg);
                binRead.read((char *)dataBuffer,4);
                binRead.close();

                //Find binary file
                if (strstr((char*)(dataBuffer), "ELF"))
                {
                     files.push_back(sub_file);
                }
            }
        }
        else if (ptr->d_type == DT_DIR) {
            if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
                getFiles(sub_file, files);
            }
        }
    }
    closedir(pDir);
}
/**
 * Creat multiple directory
 * @param directoryPath
 * @return
 */
int32_t createDirectory(const std::string &directoryPath)
{
    uint32_t dirPathLen = directoryPath.length();

    if (dirPathLen > 256)
    {
        return -1;
    }
    char tmpDirPath[256] = { 0 };

    for (uint32_t i = 0; i < dirPathLen; ++i)
    {
        tmpDirPath[i] = directoryPath[i];
        if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/')
        {
            if (ACCESS(tmpDirPath, 0) != 0)
            {
                int32_t ret = MKDIR(tmpDirPath);
                if (ret != 0)
                {
                    return ret;
                }
            }
        }
    }
    return 0;
}
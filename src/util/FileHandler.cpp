#include "FileHandler.hpp"

FileHandler::FileHandler() = default;

void FileHandler::fileDiscovery(const std::string& path, bool fullPath, const std::function<void(const std::string&)>& callback) {
    struct dirent* entry;
    DIR* dp = opendir(path.c_str());

    if (dp != nullptr) {
        while ((entry = readdir(dp))) {
            if (entry->d_name[0] == '.') {
                continue;
            }
            if (entry->d_type == DT_DIR) {
                fileDiscovery(path + "/" + entry->d_name, fullPath, callback);
            }
            if (entry->d_type == DT_REG) {
                if (fullPath) {
                    callback(path + "/" + entry->d_name);
                } else {
                    callback(entry->d_name);
                }
            }
        }
        closedir(dp);
    }
}

bool FileHandler::readBitFile(char const* path) {
    char ch;
    std::ifstream fin;
    fin.open(path, std::ios::in);
    if (!fin) {
        return false;
    }
    fin >> std::noskipws >> ch;
    fin.close();

    return ch == '1';
}

bool FileHandler::writeBitFile(char const* path, bool state) {
	std::ofstream newFile(path, std::ios_base::app);

	if (newFile.is_open()) {
		newFile << state;
	} else {
		std::cout << "Overwriting file failed" << std::endl;
		return false;
	}
    newFile.close();
	return true;
}

bool FileHandler::writeStringToFile(char const* path, char const* content) {
	std::ofstream newFile(path, std::ios_base::app);

	if (newFile.is_open()) {
		newFile << content;
	} else {
        std::cout << "overwriting file failed" << std::endl;
		return false;
	}

    newFile.close();
	return true;
}

FileHandler::~FileHandler() = default;

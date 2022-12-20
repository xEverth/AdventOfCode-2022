#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
//#include <optional>
#include <queue>
#include <numeric>
#include <algorithm>

template <int N>
constexpr auto FILE_PATH()
{
    return "C:\\Code\\AdventOfCode\\Day" + std::to_string(N) + "\\src\\" + std::to_string(N) + "input.txt";
}

//TOCHANGE:
//Use std::shared_ptr instead of raw if and when needed
//Use std::optional or polymorphism to differentiate Files and Folders in the return of addToFolder
//Save the filesize to the folders so it's not needed to call RecursiveFolderSize every time, and make sure it's not double-counted in the sum
//Use a proper algorithm to find the final solution
//PrettyPrint for the filesystem tree

class Folder// : std::enable_shared_from_this<Folder>
{
public:
    std::string foldername;
    Folder* parent;
    int filesize;
    std::deque<Folder>* children;
    Folder(std::string name, Folder* parentdir, int size, std::deque<Folder>* nullchildren = nullptr) : foldername{ name }, parent{ parentdir }, filesize{ size }, children{ nullchildren } {
        if (!nullchildren)
        {
            //std::copy(nullchildren->begin(), nullchildren->end(), children->begin());
        }
    };
    explicit Folder(Folder&& other) noexcept(true) : foldername{other.foldername}, parent{other.parent}, filesize{other.filesize}, children{std::move(other.children)} {};
    explicit Folder(const Folder& other) = delete;
    /*: foldername{ other.foldername }, parent{ other.parent }, filesize{ other.filesize } {
        std::copy(other.children->begin(), other.children->end(), std::back_inserter(*children));
    };*/
    Folder* addToFolder(std::string line) {
        std::string_view sv_line = line;
        if (sv_line.starts_with("dir "))
        {
            std::string newFoldername = line.substr(4, line.size());
            if (!children)
            {
                children = new std::deque<Folder>;
            }
            return &children->emplace_back(newFoldername, this, 0);
            //children->emplace_back(newFoldername, shared_from_this(), 0);
            //return std::make_shared<Folder>(children->back());
        }
        else
        {
            size_t wsPos = sv_line.find_first_of(' ');
            std::string newFilename = line.substr(wsPos + 1, line.size());
            int fileSize = std::stoi(line.substr(0, wsPos));
            //filesize += fileSize;
            if (!children)
            {
                children = new std::deque<Folder>;
            }
            children->emplace_back(newFilename, this, fileSize);
            return nullptr; //std::nullopt;
            //return children->emplace_back(newFilename, shared_from_this(), fileSize, std::make_unique<std::vector<Folder>>(new std::vector<Folder>()));
        }
        return nullptr; //std::nullopt;
        //return shared_from_this();
    }
    int ChildrenFilesize() {
        int childsum = 0;
        for ( auto& v : *children)
        {
        //std::cout << "Inner filesize of " << v.foldername << " = " << v.filesize << std::endl;
        childsum += v.filesize;
            if (v.children && !v.children->empty())
                childsum += v.ChildrenFilesize();
        }
        //filesize = childsum;
        return childsum;
    }
    int RecursiveFilesize() {
        //std::cout << "Checking Filesize of folder: " << this->foldername << std::endl;
        return ChildrenFilesize();
    }
    std::string fullPath() {
        std::deque<std::string> tokens{ foldername };
        //issue with nested folders
        if (auto next = parent)
        {
            do {
                tokens.push_back(next->foldername);
            } while (next = next->parent);
        std::string fullpath = std::accumulate(tokens.rbegin()+1, tokens.rend(), std::string("/"), [](std::string str, auto it) {return str + it + "/";});
        //std::cout << fullpath << std::endl;
        return fullpath;
        }
        return "/";
    }
    friend std::allocator<std::queue<Folder>>;
};

//serve tenere un puntatore alla directory attiva e popolare l'albero ogni volta che si chiama ls
//con cd si salta al parent oppure, se si dà un filename, direttamente alla cartella chiamata
//serve tenere una mappa o linked list delle cartelle
//per le sottocartelle ed i file potrebbero essere utili container stl
//mi sa che i nomi delle cartelle non sono unici. Ops.


int main(int argc, char** argv)
{
    Folder root( "/", nullptr, 0 );
    static std::unordered_map<std::string, Folder*> fileTree {
        {root.foldername,&root} //rootdir
    };
    Folder* currentDir = &root; // "/"

    std::ifstream input(FILE_PATH<7>(), std::istream::in);
    std::string buffer; //64byte string buffer, can also be char[]
    while (std::getline(input, buffer))
    {
        std::string_view command(buffer);
        if (command.starts_with("$ ls"))
        {
            while (input.peek() != '$' && std::getline(input, buffer))
            {
                auto newdir = currentDir->addToFolder(buffer); //this fails for files?
                if (newdir) {
                    fileTree[newdir->fullPath()] = newdir;
                }
                /*if (newdir.has_value())
                    fileTree[newdir->foldername] = &newdir.value();*/
            }
        }
        else if (command.starts_with("$ cd"))
        {
            if (command.contains(".."))
            {
                std::cout << command << '\n' << "CurrentDir =" << currentDir->fullPath() << '\t';
                currentDir = currentDir->parent;
                std::cout << "CurrentDir .. =" << currentDir->fullPath() << std::endl;
            }
            else
            {
                std::cout << command << '\n' << "CurrentDir =" << currentDir->fullPath() << '\t';
                std::string destPath = buffer.substr(5, buffer.length());
                if (destPath == "/")
                    currentDir = &root;
                else
                    currentDir = fileTree[currentDir->fullPath()+destPath+"/"];

                std::cout << "CurrentDir after CD:=" << currentDir->fullPath() << std::endl;
            }
        }
        else
            std::cout << "Error processing command: " << command << std::endl;
    }
    int result1 = 0;
    for (int recursive_folder_size = 0; auto& [key, val] : fileTree)
    {
        recursive_folder_size = val->RecursiveFilesize();
        if (recursive_folder_size < 100000)
        {
            std::cout << key << "HAS SIZE <100K" << std::endl;
            result1 += recursive_folder_size;
        }
        /*std::cout << key << "'s Folder Size = " << val->filesize << '\n'
                         << "Recursive Size = " << recursive_folder_size << std::endl;*/
    }
    std::cout << "Part 1 result: " << result1 << std::endl;
    int fsSize = 70000000, reqSize = 30000000;
    int occupied = root.RecursiveFilesize();
    int needToFree = reqSize - fsSize + occupied;
    auto result2 = std::min_element(fileTree.begin(), fileTree.end(), [needToFree](auto mapEntryA, auto mapEntryB) {return (mapEntryA.second->RecursiveFilesize() - needToFree > 0) ?
        (std::abs(mapEntryA.second->RecursiveFilesize() - needToFree)) < (std::abs(mapEntryB.second->RecursiveFilesize() - needToFree)) :
        false;});
    //this is an abomination and it calculates the file size again for each folder in the tree, then checks if the currently iterated on filesize is closer to the dimension we need to free
    //potentially gives the wrong result if the first entry is closer to needToFree in absolute value, but smaller (closest in absolute value)
    std::cout << "Part 2 result: " << result2->second->ChildrenFilesize() << std::endl;
}
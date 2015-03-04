#include <stdio.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char** argv)
{       
	if(argc < 4)
	{
		printf("bin2header input_file struct_name output_file\n");
		return EXIT_FAILURE;
	}
    std::string inputFile = argv[1];
    std::string structName = argv[2];
    std::string outputFile = argv[3];

    if(inputFile.empty()) {
        std::cout << "No input file specified" << std::endl;
        return EXIT_FAILURE;    
    }

    if(outputFile.empty()) {
        std::cout << "No output file specified" << std::endl;
        return EXIT_FAILURE;    
    }

    if(structName.empty()) {
        std::cout << "No structure name specified" << std::endl;
        return EXIT_FAILURE;    
    }
        
    // read input data
    FILE *f = fopen(inputFile.c_str(), "rb");
    
    if(!f) {
        std::cout << "Input file unreadable" << std::endl;
        return EXIT_FAILURE;    
    }
    
    fseek(f, 0L, SEEK_END);
    
    long fileLen = ftell(f);

    fseek(f, 0L, SEEK_SET);    
    std::vector<unsigned char> data(fileLen);    
    fread(&data[0], 1, fileLen, f);
    
    
    // create output file
    std::ofstream outFile(outputFile.c_str());
    
    if(!outFile) {
        return EXIT_FAILURE;
    }    
    
    outFile << "const int " << structName << "Len = " << fileLen << ";\n\n";
    outFile << "const unsigned char " << structName << "[] = {";//\n";
    
    // write actual data 
    const int NumWide = 32;
    char buf[12];
    outFile << "\t";
    for(size_t i = 0; i < data.size(); ++i) {
        if((i % NumWide) == 0) {
            outFile << "\n\t";
        }

        sprintf(buf, "%u", data[i]);
        outFile << buf;
        
        if(i != data.size()-1) {
            outFile << ",";
        }
        
    }
        
    outFile << "};\n";        
    
    
	return EXIT_SUCCESS;
}

#include <UnitTest++/UnitTest++.h>
#include "interface.h"
#include <string>
#include <stdexcept>

SUITE(HelpTests) {
    TEST(ShowHelpWithShortOption) {
        UserInterface iface;
        const char* argv[] = {"test", "-h", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK(!iface.Parser(argc, argv));
        CHECK(!iface.getDescription().empty());
    }
    
    TEST(ShowHelpWithLongOption) {
        UserInterface iface;
        const char* argv[] = {"test", "--help", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK(!iface.Parser(argc, argv));
        CHECK(!iface.getDescription().empty());
    }
    
    TEST(ShowHelpWhenNoArguments) {
        UserInterface iface;
        const char* argv[] = {"test", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK(!iface.Parser(argc, argv));
        CHECK(!iface.getDescription().empty());
    }
}

SUITE(ValidParametersTests) {
    TEST(ValidParametersShortForm) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "input.txt", "-r", "result.txt", 
                             "-d", "data.bin", "-t", "8080", "-a", "127.0.0.1", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK(iface.Parser(argc, argv));
        
        // ИСПРАВЛЕНО: используем правильный тип Params
        Params params = iface.getParams();
        CHECK_EQUAL("input.txt", params.inFileName);
        CHECK_EQUAL("result.txt", params.inFileResult);
        CHECK_EQUAL("data.bin", params.inFileData);
        CHECK_EQUAL(8080, params.Port);
        CHECK_EQUAL("127.0.0.1", params.Address);
    }
    
    TEST(ValidParametersLongForm) {
        UserInterface iface;
        const char* argv[] = {"test", "--input", "input.txt", "--result", "result.txt", 
                             "--data", "data.bin", "--port", "9090", "--address", "localhost", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK(iface.Parser(argc, argv));
        
        // ИСПРАВЛЕНО: используем правильный тип Params
        Params params = iface.getParams();
        CHECK_EQUAL("input.txt", params.inFileName);
        CHECK_EQUAL("result.txt", params.inFileResult);
        CHECK_EQUAL("data.bin", params.inFileData);
        CHECK_EQUAL(9090, params.Port);
        CHECK_EQUAL("localhost", params.Address);
    }
    
    TEST(MixedShortAndLongForm) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "input.txt", "--result", "result.txt", 
                             "-d", "data.bin", "--port", "7070", "-a", "192.168.1.1", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK(iface.Parser(argc, argv));
        
        // ИСПРАВЛЕНО: используем правильный тип Params
        Params params = iface.getParams();
        CHECK_EQUAL("input.txt", params.inFileName);
        CHECK_EQUAL("result.txt", params.inFileResult);
        CHECK_EQUAL("data.bin", params.inFileData);
        CHECK_EQUAL(7070, params.Port);
        CHECK_EQUAL("192.168.1.1", params.Address);
    }
}

SUITE(MissingRequiredParametersTests) {
    TEST(MissingInputFile) {
        UserInterface iface;
        const char* argv[] = {"test", "-r", "result.txt", "-d", "data.bin", 
                             "-t", "8080", "-a", "127.0.0.1", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }
    
    TEST(MissingResultFile) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "input.txt", "-d", "data.bin", 
                             "-t", "8080", "-a", "127.0.0.1", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }
    
    TEST(MissingDataFile) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "input.txt", "-r", "result.txt", 
                             "-t", "8080", "-a", "127.0.0.1", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }
    
    TEST(MissingPort) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "input.txt", "-r", "result.txt", 
                             "-d", "data.bin", "-a", "127.0.0.1", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }
    
    TEST(MissingAddress) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "input.txt", "-r", "result.txt", 
                             "-d", "data.bin", "-t", "8080", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }
}

SUITE(InvalidParametersTests) {
    TEST(InvalidPortFormat) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "input.txt", "-r", "result.txt", 
                             "-d", "data.bin", "-t", "not_a_number", "-a", "127.0.0.1", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }
    
    TEST(NegativePort) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "input.txt", "-r", "result.txt", 
                             "-d", "data.bin", "-t", "-1", "-a", "127.0.0.1", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK(iface.Parser(argc, argv));
        CHECK_EQUAL(-1, iface.getParams().Port);
    }
    
    TEST(ZeroPort) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "input.txt", "-r", "result.txt", 
                             "-d", "data.bin", "-t", "0", "-a", "127.0.0.1", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK(iface.Parser(argc, argv));
        CHECK_EQUAL(0, iface.getParams().Port);
    }
    
    TEST(LargePort) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "input.txt", "-r", "result.txt", 
                             "-d", "data.bin", "-t", "65535", "-a", "127.0.0.1", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK(iface.Parser(argc, argv));
        CHECK_EQUAL(65535, iface.getParams().Port);
    }
}

SUITE(EdgeCasesTests) {
    TEST(UnknownParameter) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "input.txt", "-r", "result.txt", 
                             "-d", "data.bin", "-t", "8080", "-a", "127.0.0.1", 
                             "-x", "unknown", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }
    
    TEST(MissingParameterValue) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "-r", "result.txt", "-d", "data.bin", 
                             "-t", "8080", "-a", "127.0.0.1", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }
    
    TEST(DifferentParameterOrder) {
        UserInterface iface;
        const char* argv[] = {"test", "-t", "8080", "-a", "127.0.0.1", 
                             "-d", "data.bin", "-i", "input.txt", "-r", "result.txt", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK(iface.Parser(argc, argv));
        
        // ИСПРАВЛЕНО: используем правильный тип Params
        Params params = iface.getParams();
        CHECK_EQUAL("input.txt", params.inFileName);
        CHECK_EQUAL("result.txt", params.inFileResult);
        CHECK_EQUAL("data.bin", params.inFileData);
        CHECK_EQUAL(8080, params.Port);
        CHECK_EQUAL("127.0.0.1", params.Address);
    }
    
    TEST(HelpWithOtherParameters) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "input.txt", "-h", "-r", "result.txt", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK(!iface.Parser(argc, argv));
    }
}

SUITE(SpecialValuesTests) {
    TEST(EmptyStringValues) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "", "-r", "", "-d", "", 
                             "-t", "8080", "-a", "", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK(iface.Parser(argc, argv));
        
        // ИСПРАВЛЕНО: используем правильный тип Params
        Params params = iface.getParams();
        CHECK_EQUAL("", params.inFileName);
        CHECK_EQUAL("", params.inFileResult);
        CHECK_EQUAL("", params.inFileData);
        CHECK_EQUAL("", params.Address);
    }
    
    TEST(FilenamesWithSpaces) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "file with spaces.txt", "-r", "result file.txt", 
                             "-d", "data file.bin", "-t", "8080", "-a", "127.0.0.1", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK(iface.Parser(argc, argv));
        
        // ИСПРАВЛЕНО: используем правильный тип Params
        Params params = iface.getParams();
        CHECK_EQUAL("file with spaces.txt", params.inFileName);
        CHECK_EQUAL("result file.txt", params.inFileResult);
        CHECK_EQUAL("data file.bin", params.inFileData);
    }
    
    TEST(IPv6Address) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "input.txt", "-r", "result.txt", 
                             "-d", "data.bin", "-t", "8080", "-a", "::1", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK(iface.Parser(argc, argv));
        CHECK_EQUAL("::1", iface.getParams().Address);
    }
    
    TEST(SpecialCharactersInFilenames) {
        UserInterface iface;
        const char* argv[] = {"test", "-i", "input_@#$%.txt", "-r", "result-file.txt", 
                             "-d", "data.file.bin", "-t", "8080", "-a", "127.0.0.1", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        
        CHECK(iface.Parser(argc, argv));
        
        // ИСПРАВЛЕНО: используем правильный тип Params
        Params params = iface.getParams();
        CHECK_EQUAL("input_@#$%.txt", params.inFileName);
        CHECK_EQUAL("result-file.txt", params.inFileResult);
        CHECK_EQUAL("data.file.bin", params.inFileData);
    }
}

int main() {
    return UnitTest::RunAllTests();
}
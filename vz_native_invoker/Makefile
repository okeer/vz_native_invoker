# Source file name.
SOURCE = vz_native_invoker
VZ_HELPERS = vz_helpers
PARSER = ActionParser
BASE = ActionBase
CLONE = CloneVMAction
LOGIN = DispatcherLoginHelper
RESIZE = ResizeVMHddAction
REFRESH = RefreshConfigAction

# Target executable file name.
# Here we are using the same name as the source file name.
TARGET = $(SOURCE)

# Path to the Parallels Virtualization SDK files.
SDK_PATH = /usr

# Relative path to the SdkWrap directory containing
# the SDK helper files. The files are used to load
# the dynamic library.

# Point to Virtuozzo 6 or 7 SDK directory
ifneq (,$(wildcard /share/parallels-virtualization-sdk/helpers/SdkWrap))
  SDK_FOLDER = parallels-virtualization-sdk
else
  SDK_FOLDER = prlsdk
endif

SDK_WRAP_PATH = /share/$(SDK_FOLDER)/helpers/SdkWrap

OBJS = SdkWrap.o $(VZ_HELPERS).o $(LOGIN).o $(BASE).o $(PARSER).o $(CLONE).o $(RESIZE).o $(REFRESH).o $(SOURCE).o

CXX = g++ -g -std=c++0x
CXXFLAGS = -DDYN_API_WRAP -I$(SDK_PATH)/include/$(SDK_FOLDER)/ -I$(SDK_PATH)/$(SDK_WRAP_PATH)
LDFLAGS = -ldl

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CXX) -o $@ $(LDFLAGS) $(OBJS) -l boost_program_options

$(SOURCE).o : $(SOURCE).cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(SOURCE).cpp

SdkWrap.o : $(SDK_PATH)/$(SDK_WRAP_PATH)/SdkWrap.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(SDK_PATH)/$(SDK_WRAP_PATH)/SdkWrap.cpp

$(VZ_HELPERS).o : $(VZ_HELPERS).cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(VZ_HELPERS).cpp
	
$(LOGIN).o : $(LOGIN).cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(LOGIN).cpp

$(BASE).o : $(BASE).cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(BASE).cpp
	
$(PARSER).o : $(PARSER).cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(PARSER).cpp
	
$(CLONE).o : $(CLONE).cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CLONE).cpp

$(RESIZE).o : $(RESIZE).cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(RESIZE).cpp

$(REFRESH).o : $(REFRESH).cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(REFRESH).cpp


clean:
	@rm -f $(OBJS) $(TARGET)

.PHONY : all clean

PYTHON=python
PROTOC=protoc

INCLUDE_DIR=../proto
SRC_DIR=.

.PHONY: all clean

all:
	$(PROTOC) -I../ --python_out $(SRC_DIR) $(INCLUDE_DIR)/vision/result.proto
	$(PYTHON) -m grpc_tools.protoc -I../ --python_out $(SRC_DIR) --grpc_python_out $(SRC_DIR) $(INCLUDE_DIR)/vision/service.proto

clean:
	$(RM) -f proto

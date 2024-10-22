#include <iostream>
#include <vector>
#include <epicsExport.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/pvStructure.h>
#include <pv/pvArray.h>
#include <pv/server.h>

using namespace epics::pvData;
using namespace epics::pvAccess;

// Create a PVStructure with a 2D array
PVStructurePtr createPVA2DArray(int rows, int cols) {
    // Create a 1D array with all elements set to 1.0
    std::vector<double> data(rows * cols, 1.0);

    // Create a PVStructure with a field for the 2D array
    PVStructurePtr pvStructure = getPVDataCreate()->createPVStructure(
        getFieldCreate()->createFieldCreate()->createField(PVDOUBLE_ARRAY));
    
    // Create and populate the PVArray
    PVArrayPtr pvArray = getPVDataCreate()->createPVArray(
        getFieldCreate()->createDoubleArray(), data.size());
    pvArray->setData(data.data(), data.size());

    // Set the PVArray in the PVStructure
    pvStructure->getSubField<PVArray>("array")->copy(*pvArray);

    return pvStructure;
}

// PVServer class to expose the PV
class MyPVServer : public PVServer {
public:
    MyPVServer() : PVServer("BL:2D:Array") {
        int rows = 256;
        int cols = 256;
        pvStructure = createPVA2DArray(rows, cols);
        put(pvStructure); // Put the structure into the PVServer
    }

    void run() {
        // Run the server
        PVServer::run();
    }

private:
    PVStructurePtr pvStructure;
};

// Main function
int main() {
    // Create the PVServer instance
    MyPVServer server;

    // Run the server
    server.run();

    return 0;
}

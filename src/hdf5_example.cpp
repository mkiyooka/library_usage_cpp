#include <iostream>
#include <string>
#include <vector>

#include <H5Cpp.h>

static void WriteHDF5() {
    std::cout << "=== Write HDF5 File ===\n";

    H5::H5File file("example.h5", H5F_ACC_TRUNC);

    // 1D double データセット
    std::vector<double> data(10);
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = static_cast<double>(i) * 0.1;
    }
    hsize_t dims[1] = {data.size()};
    H5::DataSpace dspace(1, dims);
    H5::DataSet dset = file.createDataSet("measurements", H5::PredType::NATIVE_DOUBLE, dspace);
    dset.write(data.data(), H5::PredType::NATIVE_DOUBLE);

    // 属性の書き込み
    H5::DataSpace attr_space(H5S_SCALAR);
    H5::Attribute attr = dset.createAttribute("unit", H5::StrType(0, 8), attr_space);
    attr.write(H5::StrType(0, 8), std::string("m/s     "));

    // 2D int データセット
    int matrix[3][4] = {
        {1,  2,  3,  4},
        {5,  6,  7,  8},
        {9, 10, 11, 12}
    };
    hsize_t dims2[2] = {3, 4};
    H5::DataSpace dspace2(2, dims2);
    file.createDataSet("matrix", H5::PredType::NATIVE_INT, dspace2).write(matrix, H5::PredType::NATIVE_INT);

    std::cout << "Written: example.h5\n";
    std::cout << "  dataset 'measurements' (" << data.size() << " doubles)\n";
    std::cout << "  dataset 'matrix' (3x4 ints)\n";
}

static void ReadHDF5() {
    std::cout << "\n=== Read HDF5 File ===\n";

    H5::H5File file("example.h5", H5F_ACC_RDONLY);

    H5::DataSet dset = file.openDataSet("measurements");
    H5::DataSpace dspace = dset.getSpace();
    hsize_t dims[1];
    dspace.getSimpleExtentDims(dims);

    std::vector<double> data(dims[0]);
    dset.read(data.data(), H5::PredType::NATIVE_DOUBLE);

    std::cout << "measurements (" << dims[0] << " values): ";
    for (size_t i = 0; i < std::min(dims[0], hsize_t(5)); ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << "...\n";
}

int main() {
    WriteHDF5();
    ReadHDF5();
    return 0;
}

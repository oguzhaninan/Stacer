#include "gpu_info.h"

#include "pugixml.hpp"

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <numeric>
#include <functional>

#include "command_util.h"

GpuInfo::GpuInfo()
{
    load_correct_version();
}

std::string exec_gpu(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

bool find_driver_version_xml(pugi::xml_node node)
{
    return strcmp(node.name(), "driver_version") == 0;
}

bool find_cuda_version_xml(pugi::xml_node node)
{
    return strcmp(node.name(), "cuda_version") == 0;
}

bool find_gpu_xml(pugi::xml_node node)
{
    return strcmp(node.name(), "gpu") == 0;
}

bool find_memory_xml(pugi::xml_node node)
{
    return strcmp(node.name(), "fb_memory_usage") == 0;
}

bool find_memory_tol_xml(pugi::xml_node node)
{
    return strcmp(node.name(), "total") == 0;
}

bool find_memory_used_xml(pugi::xml_node node)
{
    return strcmp(node.name(), "used") == 0;
}

bool find_gpu_utilisation_xml(pugi::xml_node node)
{
    return strcmp(node.name(), "utilization") == 0;
}

bool find_gpu_used_xml(pugi::xml_node node)
{
    return strcmp(node.name(), "gpu_util") == 0;
}

bool find_gpu_name_xml(pugi::xml_node node)
{
    return strcmp(node.name(), "product_name") == 0;
}

bool find_gpu_serial_xml(pugi::xml_node node)
{
    return strcmp(node.name(), "serial") == 0;
}

pugi::xml_document GpuInfo::getXMLnvidiasmi() const
// Load the file and return the documents for further processing
{
    std::string xml_return = exec_gpu("nvidia-smi -q -x"); // get the xml query from nvidia-smi
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_return.c_str());

    return doc;
}

struct GpuInfo::getGPUDriverVersion_nvidiasmi{
    QString operator()(GpuInfo gi) const
    {
        pugi::xml_document doc;
        doc = gi.getXMLnvidiasmi();

        return QString::fromStdString(doc.first_child().find_node(find_driver_version_xml).first_child().value());
    }
};

struct GpuInfo::getGPUCudaVersion_nvidiasmi{
    QString operator()(GpuInfo gi) const
    {
        pugi::xml_document doc;
        doc = gi.getXMLnvidiasmi();

        std::string cuda_str = "CUDA_";

        return  QString::fromStdString( cuda_str.append(doc.first_child().find_node(find_cuda_version_xml).first_child().value()));
    }
};

struct GpuInfo::getGPUPhysicalCoreCount_nvidiasmi{
    int operator()(GpuInfo gi) const
    {
        pugi::xml_document doc;
        doc = gi.getXMLnvidiasmi();

        int i = 0;

        for (pugi::xml_node gpu: doc.first_child().children("gpu"))
        {
            i+=1;
        }

        return  i;
    }
};

struct GpuInfo::getGPUMemoryUsage_nvidiasmi{
    std::vector<double> operator()(GpuInfo gi) const
    {
        pugi::xml_document doc;
        doc = gi.getXMLnvidiasmi();

        std::string local_string_gpu_memory_used;

        std::vector<double> gpu_memory_used;

        pugi::xml_node node_inside;

        for (pugi::xml_node gpu: doc.first_child().children("gpu"))
        {
            local_string_gpu_memory_used = gpu.find_node(find_memory_xml).find_node(find_memory_used_xml).first_child().value();
            gpu_memory_used.push_back(std::stod(local_string_gpu_memory_used.erase(local_string_gpu_memory_used.find(" MiB"))));
        }

        //double sum_used_memory = std::accumulate(gpu_memory_used.begin(), gpu_memory_used.end(), decltype(gpu_memory_used)::value_type(0))/1e3;

        return gpu_memory_used;

    }
};

struct GpuInfo::getGPUMemoryTotal_nvidiasmi{
    std::vector<double> operator()(GpuInfo gi) const
    {
        pugi::xml_document doc;
        doc = gi.getXMLnvidiasmi();

        std::string local_string_gpu_memory_total;

        std::vector<double> gpu_memory_total;

        pugi::xml_node node_inside;

        for (pugi::xml_node gpu: doc.first_child().children("gpu"))
        {
            local_string_gpu_memory_total = gpu.find_node(find_memory_xml).find_node(find_memory_tol_xml).first_child().value();
            gpu_memory_total.push_back(std::stod(local_string_gpu_memory_total.erase(local_string_gpu_memory_total.find(" MiB"))));
        }

        //double sum_used_memory = std::accumulate(gpu_memory_used.begin(), gpu_memory_used.end(), decltype(gpu_memory_used)::value_type(0))/1e3;

        return gpu_memory_total;

    }
};

struct GpuInfo::getGPUUsage_nvidiasmi{
    std::vector<double> operator()(GpuInfo gi) const
    {
        pugi::xml_document doc;
        doc = gi.getXMLnvidiasmi();

        std::string local_string_gpu_utilisation;

        std::vector<double> gpu_utilisation;

        pugi::xml_node node_inside;

        for (pugi::xml_node gpu: doc.first_child().children("gpu"))
        {

            //local_string_gpu_memory_total
            local_string_gpu_utilisation = gpu.find_node(find_gpu_utilisation_xml).find_node(find_gpu_used_xml).first_child().value();
            gpu_utilisation.push_back(std::stod(local_string_gpu_utilisation.erase(local_string_gpu_utilisation.find(" %"))));

        }

        //double sum_used_memory = std::accumulate(gpu_memory_used.begin(), gpu_memory_used.end(), decltype(gpu_memory_used)::value_type(0))/1e3;

        return gpu_utilisation;

    }
};

struct GpuInfo::getGPUName_nvidiasmi{
    QStringList operator()(GpuInfo gi) const
    {
        pugi::xml_document doc;
        doc = gi.getXMLnvidiasmi();

        std::string local_string_gpu_name;
        std::string local_string_gpu_serial;
        std::string space = "__";

        QStringList gpu_names;

        pugi::xml_node node_inside;

        for (pugi::xml_node gpu: doc.first_child().children("gpu"))
        {

            //local_string_gpu_memory_total
            local_string_gpu_name = gpu.find_node(find_gpu_name_xml).first_child().value();
            local_string_gpu_serial = gpu.find_node(find_gpu_serial_xml).first_child().value();
            gpu_names.push_back(QString::fromStdString(local_string_gpu_name + space + local_string_gpu_serial));

        }

        //double sum_used_memory = std::accumulate(gpu_memory_used.begin(), gpu_memory_used.end(), decltype(gpu_memory_used)::value_type(0))/1e3;

        return gpu_names;

    }
};

int GpuInfo::load_correct_version()
{

    //std::string string_response;
    std::string string_response = exec_gpu("nvidia-smi");
    size_t location = string_response.find("NVIDIA-SMI");
    bool function_running = location != std::string::npos;
    if (function_running)
    { // the nvidia-smi command is available
        this->getGPUDriverVersion = getGPUDriverVersion_nvidiasmi();
        this->getGPUAcceleratorVersion = getGPUCudaVersion_nvidiasmi();
        this->getGPUPhysicalCoreCount = getGPUPhysicalCoreCount_nvidiasmi();
        this->getGPUMemoryUsage = getGPUMemoryUsage_nvidiasmi();
        this->getGPUMemoryTotal = getGPUMemoryTotal_nvidiasmi();
        this->getGPUUsage = getGPUUsage_nvidiasmi();
        this->getGPUName = getGPUName_nvidiasmi();
        this->finding_GPU_info = true;
        return 1;
    }

    return 0;
}
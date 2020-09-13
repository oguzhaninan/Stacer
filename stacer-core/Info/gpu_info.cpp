#include "gpu_info.h"

#include "command_util.h"

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

int GpuInfo::load_correct_version() const:
{
    
}


pugi::xml_document GpuInfo::getXMLnvidiasmi() const
// Load the file and return the documents for further processing
{
    std::string xml_return = exec("nvidia-smi -q -x"); // get the xml query from nvidia-smi
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_return.c_str());

    return doc
}

std::string GpuInfo::getGPUDriverVersion_nvidiasmi() const
// Get the gpu driver version from the nvidia smi
{
    pugi::xml_document doc = GpuInfo::getXMLnvidiasmi()

    return doc.first_child().find_node(find_driver_version).first_child().value();
}

std::string GpuInfo::getGPUDriverVersion_nvidiasmi() const
// Get the gpu driver version from the nvidia smi
{
    pugi::xml_document doc = GpuInfo::getXMLnvidiasmi()

    return doc.first_child().find_node(find_cuda_version).first_child().value();
}

// int GpuInfo::getCpuPhysicalCoreCount() const
// {
//     static int count = 0;

//     if (! count) {
//         QStringList cpuinfo = FileUtil::readListFromFile(PROC_CPUINFO);

//         if (! cpuinfo.isEmpty()) {
// 	    QSet<QPair<int, int> > physicalCoreSet;
// 	    int physical = 0;
// 	    int core = 0;
// 	    for (int i = 0; i < cpuinfo.size(); ++i) {
// 	        const QString& line = cpuinfo[i];
// 		if (line.startsWith("physical id")) {
// 		    QStringList fields = line.split(": ");
// 		    if (fields.size() > 1)
// 		        physical = fields[1].toInt();
// 		}
// 		if (line.startsWith("core id")) {
// 		    QStringList fields = line.split(": ");
// 		    if (fields.size() > 1)
// 		        core = fields[1].toInt();
// 		    // We assume core id appears after physical id.
// 		    physicalCoreSet.insert(qMakePair(physical, core));
// 		}
// 	    }
// 	    count = physicalCoreSet.size();
// 	}
//     }

//     return count;
// }

// int GpuInfo::getCpuCoreCount() const
// {
//     static quint8 count = 0;

//     if (! count) {
//         QStringList cpuinfo = FileUtil::readListFromFile(PROC_CPUINFO);

//         if (! cpuinfo.isEmpty())
//             count = cpuinfo.filter(QRegExp("^processor")).count();
//     }

//     return count;
// }

// QList<double> GpuInfo::getLoadAvgs() const
// {
//     QList<double> avgs = {0, 0, 0};

//     QStringList strListAvgs = FileUtil::readStringFromFile(PROC_LOADAVG).split(QRegExp("\\s+"));

//     if (strListAvgs.count() > 2) {
//         avgs.clear();
//         avgs << strListAvgs.takeFirst().toDouble();
//         avgs << strListAvgs.takeFirst().toDouble();
//         avgs << strListAvgs.takeFirst().toDouble();
//     }

//     return avgs;
// }

// double GpuInfo::getAvgClock() const
// {
//     const QStringList lines = CommandUtil::exec("bash",{"-c", LSCPU_COMMAND}).split('\n');
//     const QString clockMHz = lines.filter(QRegExp("^CPU MHz")).first().split(":").last();
//     return clockMHz.toDouble();
// }

// QList<double> GpuInfo::getClocks() const
// {
//     QStringList lines = FileUtil::readListFromFile(PROC_CPUINFO)
//             .filter(QRegExp("^cpu MHz"));

//     QList<double> clocks;
//     for(auto line: lines){
//         clocks.push_back(line.split(":").last().toDouble());
//     }
//     return clocks;
// }

// QList<int> GpuInfo::getCpuPercents() const
// {
//     QList<double> cpuTimes;

//     QList<int> cpuPercents;

//     QStringList times = FileUtil::readListFromFile(PROC_STAT);

//     if (! times.isEmpty())
//     {
//      /*  user nice system idle iowait  irq  softirq steal guest guest_nice
//         cpu  4705 356  584    3699   23    23     0       0     0      0
//          .
//         cpuN 4705 356  584    3699   23    23     0       0     0      0

//           The meanings of the columns are as follows, from left to right:
//              - user: normal processes executing in user mode
//              - nice: niced processes executing in user mode
//              - system: processes executing in kernel mode
//              - idle: twiddling thumbs
//              - iowait: waiting for I/O to complete
//              - irq: servicing interrupts
//              - softirq: servicing softirqs
//              - steal: involuntary wait
//              - guest: running a normal guest
//              - guest_nice: running a niced guest
//         */

//         QRegExp sep("\\s+");
//         int count = GpuInfo::getCpuCoreCount() + 1;
//         for (int i = 0; i < count; ++i)
//         {
//             QStringList n_times = times.at(i).split(sep);
//             n_times.removeFirst();
//             for (const QString &t : n_times)
//                 cpuTimes << t.toDouble();

//             cpuPercents << getCpuPercent(cpuTimes, i);

//             cpuTimes.clear();
//         }
//     }

//     return cpuPercents;
// }

// int GpuInfo::getCpuPercent(const QList<double> &cpuTimes, const int &processor) const
// {
//     const int N = getCpuCoreCount()+1;

//     static QVector<double> l_idles(N);
//     static QVector<double> l_totals(N);

//     int utilisation = 0;

//     if (cpuTimes.count() > 0) {

//         double idle = cpuTimes.at(3) + cpuTimes.at(4); // get (idle + iowait)
//         double total = 0.0;
//         for (const double &t : cpuTimes) total += t; // get total time

//         double idle_delta  = idle  - l_idles[processor];
//         double total_delta = total - l_totals[processor];

//         if (total_delta)
//             utilisation = 100 * ((total_delta - idle_delta) / total_delta);

//         l_idles[processor] = idle;
//         l_totals[processor] = total;
//     }

//     if (utilisation > 100) utilisation = 100;
//     else if (utilisation < 0) utilisation = 0;

//     return utilisation;
// }

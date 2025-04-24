#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <sys/sysinfo.h>
#include <unistd.h>
//#include <curses.h>
#include <algorithm>

std::string int_to_str(int val){
    std::string s_int;
    if (val < 10){
        std::stringstream sstr;
        sstr<<"0"<<val;
        s_int = sstr.str();
    }
    else {
        std::stringstream sstr;
        sstr<<val;
        s_int = sstr.str();
    }
    return s_int;
    }

std::string get_time(){
    std::time_t now = time(NULL);
    std::tm* nowLocal = std::localtime(&now);
    std::string time = int_to_str(nowLocal->tm_hour) + ":" + int_to_str(nowLocal->tm_min) + ":" + int_to_str(nowLocal->tm_sec);
    return time;
}

std::string get_timee(){
    std::time_t now = time(NULL);
    std::tm* nowLocal = std::localtime(&now);
    std::string time = int_to_str(nowLocal->tm_hour) +  int_to_str(nowLocal->tm_min) +  int_to_str(nowLocal->tm_sec);
    return time;
}

std::string get_date(){
    std::time_t now = time(NULL);
    std::tm* nowLocal = std::localtime(&now);
    std::string date = int_to_str(nowLocal->tm_mday) + int_to_str(nowLocal->tm_mon+1) + int_to_str(nowLocal->tm_year + 1900);
    return date;
}

double get_cpu_usage(){
	std::ifstream file("/proc/stat");
	std::string line;
	std::getline(file, line);
	file.close();

	std::istringstream iss(line);	
	std::string cpu;
	int user, nice, system, idle, iowait, irq, softirq, steal, quest, quest_nice;
	iss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> quest >> quest_nice;
	
	int idle_time = idle + iowait;
	int total_time = user + nice + system + idle_time + irq + softirq + steal;
	double usage = 100.0 * (total_time - idle_time)/total_time;

	return usage;
}

long get_totalram(){
	const double megabyte = 1024*1024;
	struct sysinfo si;
	sysinfo(&si);
	unsigned long total_ram = si.totalram/megabyte;
	
	return total_ram;
}

long get_freeram(){
	const double megabyte = 1024*1024;
	struct sysinfo si;
	sysinfo(&si);
	unsigned long free_ram = si.freeram/megabyte;

	return free_ram;
}

long get_ram(){
	long total_ram = get_totalram();
	long free_ram = get_freeram();
	long ram = total_ram - free_ram;
	
	return ram;
	}
std::string get_ip(){
	FILE *fp;
	char ip_ch[256];
	std::string ip;

	fp = popen("ifconfig eth0 | grep 'inet ' | awk '{print $2}'", "r");
	if (fp != NULL){
		fgets(ip_ch, 256, fp);
	}
	pclose(fp);
	ip = ip_ch;
	ip.erase(remove_if(ip.begin(), ip.end(), isspace), ip.end());
	return ip;
}
std::string get_lo_ip(){
	FILE *fp;
	char l_ip_ch[256];
	std::string l_ip;
	
	fp = popen("ifconfig lo | grep 'inet ' | awk '{print $2}'", "r");
	if (fp != NULL){
		fgets(l_ip_ch, 256, fp);
	}
	l_ip = l_ip_ch;
	l_ip.erase(remove_if(l_ip.begin(), l_ip.end(), isspace), l_ip.end());
	pclose(fp);
	return l_ip;
}

std::string get_netmask(){
	FILE *fp;
	char nm_ch[256];
	std::string nm;	
	fp = popen("ifconfig lo | grep 'netmask' | awk '{print $4}'", "r");
	if (fp != NULL){
		fgets(nm_ch, 256, fp);
	}
	nm = nm_ch;
	nm.erase(remove_if(nm.begin(), nm.end(), isspace), nm.end());
	pclose(fp);
	return nm;
}

std::string get_freedu(){
	FILE *fp;
	char freedu_ch[256];
	std::string freedu;
	
	fp = popen("df -h --output=avail --total", "r");
	if (fp != NULL){
		while(fgets(freedu_ch, 256, fp)!= NULL){
		}
	}	
	freedu = freedu_ch;	
	pclose(fp);
	freedu.erase(remove_if(freedu.begin(), freedu.end(), isspace), freedu.end());
	return freedu;
}

std::string get_fulldu(){
	FILE *fp;
	char fulldu_ch[256];
	std::string fulldu;
	fp = popen("df -h --output=size --total", "r");
	if (fp != NULL){
		while(fgets(fulldu_ch, 256, fp)!= NULL){
		}
	}	
		
	pclose(fp);
	fulldu = fulldu_ch;
	fulldu.erase(remove_if(fulldu.begin(), fulldu.end(), isspace), fulldu.end());

	return fulldu;
}

void on_screen(){
	std::cout<<get_time()<<std::endl;
	std::cout<<"Total CPU usage: "<<get_cpu_usage()<<"%"<<std::endl;
	std::cout<<"Disk free: "<<get_freedu()<<std::endl;
	std::cout<<"Disk total: "<<get_fulldu()<<std::endl;
	std::cout<<"total RAM: "<<get_totalram()<<"Mb"<<std::endl;
	std::cout<<"RAM: "<<get_ram()<<"Mb"<<std::endl;
	std::cout<<"ip adress: "<<get_ip()<<std::endl;
	std::cout<<"lo ip adress: "<<get_lo_ip()<<std::endl;
	std::cout<<"netmask lo: "<<get_netmask()<<std::endl;
	std::cout<<"\n"<<std::endl;
}



void body_log(std::string logname){
//    std::string logname = get_date() + "_" + get_timee();
    const char * c = logname.c_str();
    FILE* log = fopen(c, "a");
    if (!log){
        printf("for some reason there's error");
    }
    else
    {
		std::string time = get_time();
		double cpu = get_cpu_usage();
		std::string freedu = get_freedu();
		std::string fulldu = get_fulldu();
		long totalram = get_totalram();
		long ram = get_ram();
		std::string ip = get_ip();
		std::string lo_ip = get_lo_ip();
		std::string netmask = get_netmask(); 

		fprintf(log, "%s; ", time.c_str());
	        fprintf(log, "%f; ", cpu);
        	fprintf(log,"%s; ", freedu.c_str());
	        fprintf(log,"%s; ", fulldu.c_str());
	        fprintf(log,"%u; ", totalram);
	        fprintf(log,"%u; ", ram);
	        fprintf(log,"%s; ", ip.c_str());
	        fprintf(log,"%s; ", lo_ip.c_str());
	        fprintf(log,"%s; ", netmask.c_str());
		fprintf(log, "\n");
	}
    fclose(log);
}

void cpu_logfile(std::string logname){
    const char * ccpu = logname.c_str();
    FILE* cpu_log = fopen(ccpu, "a");
	if (!cpu_log){
		printf("for some reason there's error");
	}
else
	{
        std::string time = get_time();
        double cpu = get_cpu_usage();

        fprintf(cpu_log, "%s; ", time.c_str());
        fprintf(cpu_log, "%f ", cpu);
        fprintf(cpu_log, "\n");
	}
    fclose(cpu_log);
}

void ram_logfile(std::string logname){
    const char * cram = logname.c_str();
    FILE* ram_log = fopen(cram, "a");
    if (!ram_log){
        printf("for some reason there's error");
    }
    else
    {
        std::string time = get_time();
        unsigned long ram = get_ram();

        fprintf(ram_log, "%s; ", time.c_str());
        fprintf(ram_log, "%u ", ram);
        fprintf(ram_log, "\n");
    }
    fclose(ram_log);
}


int main(){
    std::string global_logname = get_date() + "_" + get_timee() + ".csv";
    const char * c = global_logname.c_str();
    FILE* log = fopen(c, "a");
    if (!log) {
        std::cout<<"for some reason can't open file";
    }
    else {
        fprintf(log, "time; ");
        fprintf(log, "cpu %%; ");
        fprintf(log,"disk free; ");
        fprintf(log,"all disk; ");
        fprintf(log,"total RAM; ");
        fprintf(log,"RAM; ");
        fprintf(log,"ip; ");
        fprintf(log,"local ip; ");
        fprintf(log,"netmask");
        fprintf(log, "\n");
    }
    fclose(log);

    std::string cpu_logname = "cpu_" + get_date() + "_" + get_timee() + ".csv";
    const char * ccpu = cpu_logname.c_str();
    FILE* cpu_log = fopen(ccpu, "a");
    if (!cpu_log) {
        std::cout<<"for some reason can't open file";
    }
    fclose(cpu_log);

    std::string ram_logname = "ram_" + get_date() + "_" + get_timee() + ".csv";
    const char * cram = ram_logname.c_str();
    FILE* ram_log = fopen(cram, "a");
    if (!ram_log) {
        std::cout<<"for some reason can't open file";
    }
    fclose(ram_log);

    bool stop = false;
	while (!stop){
		on_screen();
	        body_log(global_logname);
	        cpu_logfile(cpu_logname);
	        ram_logfile(ram_logname);
		sleep(5);
	}
}

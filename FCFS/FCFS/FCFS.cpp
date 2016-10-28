#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

struct Process {												//A class called Process with a list of member variables
	int start_time = 0;
	int end_time = 0;
	int time_CPU = 0;
	int time_IO = 0;
	int CPU_Burst = 0;
	int IO_Burst = 0;
	int waiting_time = 0;
	int process_number = 0;
	int response_time = 0;
	bool isDone = false;
	vector <int> data ;
};

int Calc_Waiting_Time(int, int);
float Calc_CPU_Utilization(float, float);

int main() {

	int data[8][19] = { { 4,24,		5,73,	3,31,	5,27,	4,33,	6,43,	4,64,	5,19,	2 },				//Initialization of all processes in a 2-D Array
						{ 18,31,	19,35,	11,42,	18,43,	19,47,	18,43,	17,51,	19,32,	10 },
						{ 6,18,		4,21,	7,19,	4,16,	5,29,	7,21,	8,22,	6,24,	5 },
						{ 17,42,	19,55,	20,54,	17,52,	15,67,	12,72,	15,66,	14 },
						{ 5,81,		4,82,	5,71,	3,61,	5,62,	4,51,	3,77,	4,61,	3,42,	5 },
						{ 10,35,	12,41,	14,33,	11,32,	15,41,	13,29,	11 },
						{ 21,51,	23,53,	24,61,	22,31,	21,43,	20 },
						{ 11,52,	14,42,	15,31,	17,21,	16,43,	12,31,	13,32,	15 }
	};

	vector <Process> vProcess;
	vector <Process> ReadyQ;
	vector <Process> doneProcess;

	struct Process P[8];					//Declaration of an Object of an array of Process called "P"
	struct Process tempProcess;

	int last_CPU_endtime = 0;
	int next_up;
	int position;
	int size;
	int index;
	int time = 0;
	int counter = 0;
	int total_waiting = 0;
	int total_turnaround = 0;
	int total_response = 0;
	float total_CPU_Bursts = 0;
	float time_completion;
	bool skip = false;

	//Read in data and put it in the Process
	for (signed int i = 0; i < 8; i++) {
		//Assign Process Number
		P[i].process_number = i;
		for (signed int j = 18; j >= 0; j--) {
			//Store data into the Process'
			if (data[i][j] != 0)
				P[i].data.push_back(data[i][j]);
		}
	}

	//Initial iteration
	P[0].CPU_Burst = P[0].data.back();
	P[0].data.pop_back();
	total_CPU_Bursts += P[0].CPU_Burst;
	P[0].IO_Burst = P[0].data.back();
	P[0].data.pop_back();
	P[0].start_time = time;
	P[0].response_time = P[0].waiting_time = 0;
	P[0].time_CPU = P[0].start_time + P[0].CPU_Burst;
	P[0].end_time = P[0].time_IO = P[0].time_CPU + P[0].IO_Burst;
	vProcess.push_back(P[0]);
	for (unsigned int j = 1; j < 8; j++) {
		P[j].CPU_Burst = P[j].data.back();
		P[j].data.pop_back();
		total_CPU_Bursts += P[j].CPU_Burst;
		P[j].IO_Burst = P[j].data.back();
		P[j].data.pop_back();
		P[j].start_time = P[j - 1].time_CPU;
		P[j].response_time = P[j].waiting_time += Calc_Waiting_Time(P[j].start_time, P[j].end_time);
		time = P[j].time_CPU = P[j].start_time + P[j].CPU_Burst;
		P[j].time_IO = P[j].time_CPU + P[j].IO_Burst;
		P[j].end_time = P[j].time_IO;
		vProcess.push_back(P[j]);
	}

	//Main Iteration
	while (doneProcess.size() != 8) {

		//Find next_up
		next_up = vProcess[0].end_time;
		position = 0;
		for (unsigned int i = 1; i < vProcess.size(); i++) {
			if (vProcess[i].end_time < next_up) {
				next_up = vProcess[i].end_time;
				position = i;
			}
		}

		//Delete from vProcess and then Push it to ReadyQ, like we were to move content
		ReadyQ.push_back(vProcess[position]);
		vProcess.erase(vProcess.begin() + position);

		//Execute ReadyQ
		for (unsigned int j = 0; j < ReadyQ.size(); j++) {
			//If process is still in IO time when it is Ready to Execute, then it will start after it finishes it's IO
				total_CPU_Bursts += ReadyQ[j].CPU_Burst = ReadyQ[j].data.back();
				ReadyQ[j].data.pop_back();
				if (ReadyQ[j].data.empty() == true) {
					ReadyQ[j].isDone = true;
					ReadyQ[j].IO_Burst = 0;
					ReadyQ[j].start_time = time;
					if (ReadyQ[j].start_time < ReadyQ[j].end_time)
						ReadyQ[j].start_time = ReadyQ[j].end_time;
					ReadyQ[j].waiting_time += Calc_Waiting_Time(ReadyQ[j].start_time, ReadyQ[j].end_time);
					ReadyQ[j].time_CPU = ReadyQ[j].start_time + ReadyQ[j].CPU_Burst;
					ReadyQ[j].time_IO = ReadyQ[j].time_CPU + ReadyQ[j].IO_Burst;
					ReadyQ[j].end_time = ReadyQ[j].time_IO;
					time = last_CPU_endtime = ReadyQ[j].time_CPU;
					vProcess.push_back(ReadyQ[j]);
				}
				else {
					ReadyQ[j].IO_Burst = ReadyQ[j].data.back();
					ReadyQ[j].data.pop_back();
					ReadyQ[j].start_time = time;
					if (ReadyQ[j].start_time < ReadyQ[j].end_time)
						ReadyQ[j].start_time = ReadyQ[j].end_time;
					ReadyQ[j].waiting_time += Calc_Waiting_Time(ReadyQ[j].start_time, ReadyQ[j].end_time);
					ReadyQ[j].time_CPU = ReadyQ[j].start_time + ReadyQ[j].CPU_Burst;
					ReadyQ[j].time_IO = ReadyQ[j].time_CPU + ReadyQ[j].IO_Burst;
					ReadyQ[j].end_time = ReadyQ[j].time_IO;
					time = last_CPU_endtime = ReadyQ[j].time_CPU;
					vProcess.push_back(ReadyQ[j]);
				}
	}

	//Clear ReadyQ
	ReadyQ.clear();

	//Check which process is done executing all its data
	for (unsigned int j = 0; j < vProcess.size(); j++) {
		if (vProcess[j].isDone == true) {
			doneProcess.push_back(vProcess[j]);
			vProcess.erase(vProcess.begin() + j);
			//Starts at position where you might have deleted a Process
			j--;
		}
	}
}

	cout << "\t\t\tFCFS Scheduling Algorithm\n";
	cout << "\tWaiting Time\tTurnaround Time\t\tResponse Time" << endl;
	//Sort Done Process
	for (unsigned int j = 0; j < doneProcess.size(); j++) {
		//Sort Process by Number
		index = 0;
		while (doneProcess[index].process_number != j)
			index++;
		cout << "P[" << doneProcess[index].process_number + 1 << "]:\t" << doneProcess[index].waiting_time << "ms";
		cout << "\t\t" << doneProcess[index].end_time << "ms";
		cout << "\t\t\t" << doneProcess[index].response_time << "ms\n";
	}

	
	time_completion = (float) doneProcess[0].end_time;
	total_waiting += doneProcess[0].waiting_time;
	total_turnaround += doneProcess[0].end_time;
	total_response += doneProcess[0].response_time;
	for (unsigned int i = 1; i < doneProcess.size(); i++) {
		total_waiting += doneProcess[i].waiting_time;
		total_turnaround += doneProcess[i].end_time;
		total_response += doneProcess[i].response_time;
		if (doneProcess[i].end_time > time_completion)
			time_completion = (float) doneProcess[i].end_time;
	}

	cout << "Avg:\t" << (float)total_waiting / 8 << "ms\t" << (float)total_turnaround / 8 << "ms\t\t\t" << (float)total_response / 8 << "ms\n";
	cout << "FCFS CPU Utilization: " << setprecision(4) << Calc_CPU_Utilization(total_CPU_Bursts, time_completion) << "%" << endl;
	counter = 0;
	
	return 0;
}

int Calc_Waiting_Time(int start, int end) {
	return start - end;
}

float Calc_CPU_Utilization(float total_CPU_Burst, float time_completion) {
	return ((total_CPU_Burst / time_completion) * 100);
}
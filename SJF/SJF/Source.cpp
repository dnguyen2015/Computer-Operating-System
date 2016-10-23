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
};

int Calc_Waiting_Time(int, int);
float Calc_CPU_Utilization(float, float);

int main() {

	int p[8][19] = { { 4,24,		5,73,	3,31,	5,27,	4,33,	6,43,	4,64,	5,19,	2 },				//Initialization of all processes in a 2-D Array
	{ 18,31,	19,35,	11,42,	18,43,	19,47,	18,43,	17,51,	19,32,	10 },
	{ 6,18,		4,21,	7,19,	4,16,	5,29,	7,21,	8,22,	6,24,	5 },
	{ 17,42,	19,55,	20,54,	17,52,	15,67,	12,72,	15,66,	14 },
	{ 5,81,		4,82,	5,71,	3,61,	5,62,	4,51,	3,77,	4,61,	3,42,	5 },
	{ 10,35,	12,41,	14,33,	11,32,	15,41,	13,29,	11 },
	{ 21,51,	23,53,	24,61,	22,31,	21,43,	20 },
	{ 11,52,	14,42,	15,31,	17,21,	16,43,	12,31,	13,32,	15 }
	};

	vector <Process> vProcess;
	vector <Process> doneProcess;

	struct Process P[8];					//Declaration of an Object of an array of Process called "P"
	struct Process tempProcess;

	int last_CPU_endtime = 0;
	int next_up = 0;
	int position = 0;
	int size = 0;
	int index = 0;
	int counter = 0;
	int total_waiting = 0;
	int total_turnaround = 0;
	int total_response = 0;
	float total_CPU_Bursts = 0;
	float time_completion = 0;
	bool skip = false;
	bool sorted = false;

	//Initial iteration
	for (int i = 0; i < 1; i++) {
		if (i % 2 == 0) {
			for (int j = 0; j < 8; j++) {
					P[j].CPU_Burst = p[j][i];
					P[j].IO_Burst = p[j][i + 1];
					P[j].process_number = j;
					vProcess.push_back(P[j]);
			}
		}
	}

	//Sorts Process according to SJF
	size = vProcess.size();
	for (unsigned int k = 0; k < vProcess.size(); k++) {
		//find the smallest io in vProcess
		position = 0;
		next_up = vProcess[0].CPU_Burst;
		for (signed int a = 1; a < size; a++) {
			if (vProcess[a].CPU_Burst < next_up) {
				next_up = vProcess[a].CPU_Burst;
				position = a;
			}
		}
		//Use tempProcess to copy vProcess[position] essentially moving it to the back
		tempProcess = vProcess[position];
		vProcess.erase(vProcess.begin() + position);
		vProcess.push_back(tempProcess);
		//size-- so that we ignore what we just pushed to the back iterating all the processes from previous ReadyQ
		size--;
	}

	//Execute Process according to the sort
	for (unsigned int j = 0; j < vProcess.size(); j++) {
		//If process is still in IO time when it is Ready to Execute, then it will start after it finishes it's IO
		vProcess[j].response_time = vProcess[j].start_time = last_CPU_endtime;
		if (vProcess[j].start_time < vProcess[j].end_time)
			vProcess[j].response_time = vProcess[j].start_time = vProcess[j].end_time;
		vProcess[j].waiting_time += Calc_Waiting_Time(vProcess[j].start_time, vProcess[j].end_time);
		vProcess[j].time_CPU = vProcess[j].start_time + vProcess[j].CPU_Burst;
		vProcess[j].time_IO = vProcess[j].time_CPU + vProcess[j].IO_Burst;
		vProcess[j].end_time = vProcess[j].time_IO;
		last_CPU_endtime = vProcess[j].time_CPU;
	}

	
	//Main iteration
	for (int i = 2; i < 19; i++) {
		size = vProcess.size();
		if (i % 2 == 0) {
			//Put CPU and IO Burst Times in it's respective Process
			for (int j = 0; j < 8; j++) {
				index = 0;
				while (vProcess[index].process_number != P[j].process_number) {
					if (index == vProcess.size() - 1) {
						skip = true;
						break;
					}
					else
						index++;
				}
				if (skip == true)
					//do nothing
					skip = false;
				else {
					vProcess[index].CPU_Burst = P[j].CPU_Burst = p[j][i];
					total_CPU_Bursts += p[j][i];
					if (i + 1 != 19) {
						vProcess[index].IO_Burst = P[j].IO_Burst = p[j][i + 1];
						if (vProcess[index].IO_Burst == 0) {
							vProcess[index].isDone = true;
						}
					}
					else {
						vProcess[index].isDone = true;
						vProcess[index].IO_Burst = 0;
					}
				}
			}

			size = vProcess.size();
			//Sorts Process according to SJF
			for (unsigned int k = 0; k < vProcess.size(); k++) {
				next_up = vProcess[0].CPU_Burst;
				position = 0;
				for (signed int j = 1; j < size; j++) {
					if (vProcess[j].CPU_Burst < next_up) {
						next_up = vProcess[j].CPU_Burst;
						position = j;
					}
				}
				tempProcess = vProcess[position];
				vProcess.erase(vProcess.begin() + position);
				vProcess.push_back(tempProcess);
				size--;
			}

			//Execute Process according to the sort
			for (unsigned int j = 0; j < vProcess.size(); j++) {
				//If process is still in IO time when it is Ready to Execute, then it will start after it finishes it's IO
				vProcess[j].start_time = last_CPU_endtime;
				if (vProcess[j].start_time < vProcess[j].end_time)
					vProcess[j].start_time = vProcess[j].end_time;
				vProcess[j].waiting_time += Calc_Waiting_Time(vProcess[j].start_time, vProcess[j].end_time);
				vProcess[j].time_CPU = vProcess[j].start_time + vProcess[j].CPU_Burst;
				vProcess[j].time_IO = vProcess[j].time_CPU + vProcess[j].IO_Burst;
				vProcess[j].end_time = vProcess[j].time_IO;
				last_CPU_endtime = vProcess[j].time_CPU;
			}

			//Check which process is done executing all its data
			for (unsigned int j = 0; j < vProcess.size(); j++) {
				if (vProcess[j].isDone == true) {
					doneProcess.push_back(vProcess[j]);
					vProcess.erase(vProcess.begin() + j);
					//Starts at position where you might have deleted a Process
					j--;
				}
			}
			size = vProcess.size();
		}
	}

	cout << "\t\t\tSJF Scheduling Algorithm\n";
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

	cout << "Avg:\t" << (float)total_waiting / 8 << "ms\t" << (float)total_turnaround / 8 << "ms\t\t" << (float)total_response / 8 << "ms\n";
	cout << "SJF CPU Utilization: " << setprecision(4) << Calc_CPU_Utilization(total_CPU_Bursts, time_completion) << "%" << endl;
	
	return 0;
}

int Calc_Waiting_Time(int start, int end) {
	return start - end;
}

float Calc_CPU_Utilization(float total_CPU_Burst, float time_completion) {
	return ((total_CPU_Burst / time_completion) * 100);
}
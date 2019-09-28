#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "functions.h"

using namespace std;

int search_id(int, int, int, block, FILE*);
void writesomedata(int& casework, int& current_work, int& k, int& casek, FILE* base_data, block& data)
{
	if (casework != current_work) {
		current_work = casework;
		casek = k;
		fseek(base_data, sizeof(struct block) * casework, SEEK_SET);
		fread(&data, sizeof(struct block), 1, base_data);
		cout << "Write to mem" << endl;
	}
	cout << "Enter name, surname, otchestvo, id, group" << endl;
	cin >> data.stud[casek].name >> data.stud[casek].sur >> data.stud[casek].twoname;
	cin >> data.stud[casek].id >> data.stud[casek].group;

	if (search_id(casework, casek, data.stud[casek].id, data, base_data) != -1) {
		cout << "Data are repeated" << endl;
		return;
	}
	data.stud[casek].del = true;
	casek++;
	fseek(base_data, sizeof(struct block) * casework, SEEK_SET);
	fwrite(&data, sizeof(struct block), 1, base_data);
	cout << "Write to mem" << endl;
	if (casek == 5) {
		casework++;
		memset(&data, '\0', sizeof(block));
		casek = 0;
	}
	cout << "End" << endl;
	current_work = casework;
	k = casek;
}
void showmydata(FILE* base_data, int cur_work)
{
	block data;
	fseek(base_data, 0, SEEK_SET);
	while (!feof(base_data)) {
		cout << "------------------------------------------------" << endl;
		memset(&data, '\0', sizeof(block));
		fread(&data, sizeof(struct block), 1, base_data);
		for (int i = 0; i < max_block; i++)
		{
			if (data.stud[i].del == true) {
				cout << data.stud[i].name << " " << data.stud[i].sur << " " << data.stud[i].twoname;
				cout << " " << data.stud[i].id << " " << data.stud[i].group << endl;
			}
		}
	}
}
int search_id(int cur_work, int k, int id, block data, FILE* base_data)
{
	for (int i = 0; i < max_block; i++)
		if (data.stud[i].id == id && i != k && data.stud[i].del) return i;
	fseek(base_data, 0, SEEK_SET);
	int now_work = -1;
	while (!feof(base_data)) {
		fread(&data, sizeof(struct block), 1, base_data);
		now_work++;
		for (int i = 0; i < max_block; i++)
			if (data.stud[i].id == id &&
				i != k && now_work != cur_work && data.stud[i].del) return i;
	}
	return -1;
}
int search_student(int& cur_work, int id, block& data, FILE* base_data)
{
	for (int i = 0; i < max_block; i++)
		if (data.stud[i].id == id) return i;
	fseek(base_data, 0, SEEK_SET);
	memset(&data, '\0', sizeof(block));
	fread(&data, sizeof(struct block), 1, base_data);
	int now_work = 0;
	while (data.stud[0].del != false) {
		if (now_work == cur_work) continue;
		for (int i = 0; i < max_block; i++)
			if (data.stud[i].id == id) {
				cur_work = now_work;
				return i;
			}
		fread(&data, sizeof(struct block), 1, base_data);
		now_work++;
	}
	return -1;
}
int trashwork(int& casek, int& current_work, int& casework, int& k, FILE* base_data, block& data) {
	casek--;
	current_work = 0;
	block casedata;
	bool booldata, boolcase;
	memset(&data, '\0', sizeof(block));
	memset(&casedata, '\0', sizeof(block));
	fseek(base_data, sizeof(block) * casework, SEEK_SET);
	fread(&casedata, sizeof(struct block), 1, base_data);
	fseek(base_data, 0, SEEK_SET);
	fread(&data, sizeof(struct block), 1, base_data);
	while (true) {

		booldata = false; boolcase = false;
		while (data.stud[0].name[0] != '\0') {
			for (k = 0; k < 5; k++)
			{
				if (data.stud[k].del == false) {
					booldata = true;
					goto exit1;
				}
			}
			current_work++;
			memset(&data, '\0', sizeof(block));
			fseek(base_data, sizeof(block) * current_work, SEEK_SET);
			fread(&data, sizeof(struct block), 1, base_data);
		}
	exit1:
		while (true)
		{
			for (casek = 4; casek > -1; casek--)
			{
				if (casedata.stud[casek].del == true) {
					boolcase = true;
					goto exit2;
				}
			}
			casework--;
			if (casework == -1) break;
			memset(&casedata, '\0', sizeof(block));
			fseek(base_data, sizeof(block) * casework, SEEK_SET);
			fread(&casedata, sizeof(struct block), 1, base_data);
		}
	exit2:
		if (booldata && boolcase && current_work <= casework) {
			if (current_work == casework)
				if (k > casek) break;
			cout << "Move block " << casework << " num " << casek << endl;
			cout << "To " << current_work << " num " << k << endl;
			data.stud[k] = casedata.stud[casek];
			casedata.stud[casek].del = false;
			fseek(base_data, sizeof(block) * current_work, SEEK_SET);
			fwrite(&data, sizeof(struct block), 1, base_data);
			fseek(base_data, sizeof(block) * casework, SEEK_SET);
			fwrite(&casedata, sizeof(struct block), 1, base_data);
			if (casek == 0) {
				int cur_page = casework;
				FILE* temp_file;
				temp_file = fopen("temp_data.bin", "w+");
				block temp_data;
				fseek(base_data, 0, SEEK_SET);
				for (int i = 0; i < cur_page; i++)
				{
					memset(&data, '\0', sizeof(block));
					fread(&temp_data, sizeof(struct block), 1, base_data);
					fwrite(&temp_data, sizeof(struct block), 1, temp_file);
				}
				char newname[] = "temp_data.bin";
				char name[] = "base_data.bin";
				fclose(base_data);
				fclose(temp_file);
				if (remove(name)) {
					perror("Error: ");
					cout << endl;
					/*fclose(base_data);
					return 1; */
				}
				if (rename(newname, name)) {
					perror("Error: ");
					cout << endl;
					/*fclose(base_data);
					return 1; */
				}
				base_data = fopen("base_data.bin", "r+");
			}
		}
		else break;
	}

	current_work = 0; k = 0;
	fseek(base_data, 0, SEEK_SET);
	memset(&data, '\0', sizeof(block));
	fread(&data, sizeof(struct block), 1, base_data);
	while (data.stud[0].del != false) {
		current_work++;
		memset(&data, '\0', sizeof(block));
		fread(&data, sizeof(struct block), 1, base_data);
	}
	/*memset(&data, '\0', sizeof(block));
	fseek(base_data, sizeof(block)* current_work, SEEK_SET);
	fwrite(&data, sizeof(struct block), 1, base_data); */
	if (current_work != 0) current_work--;
	fseek(base_data, sizeof(struct block) * current_work, SEEK_SET);
	fread(&data, sizeof(struct block), 1, base_data);
	while (data.stud[k].del && k != 4) k++;

	if (k == 4) {
		current_work++;
		memset(&data, '\0', sizeof(block));
		k = 0;
	}
	casek = k;
	casework = current_work;
	cout << "k = " << k << endl;
	cout << "block = " << casework << endl;
	return 0;
}
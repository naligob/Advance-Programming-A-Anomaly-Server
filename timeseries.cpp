#include "timeseries.h"

TimeSeries::TimeSeries(const char* CSVfileName) {
		ifstream data(openFile(CSVfileName));
		insertColums(data);
		tableTranspose();
		data.close();	
	}
void TimeSeries::insertColums(ifstream& data) {
	string currentLine,columName;
	vector<string> stringVec;
	vector<float> floatVec;
	bool flag = false;
	while(getline(data,currentLine)) {
		stringstream firstLine(currentLine);
		while (getline(firstLine,columName,','))
		{
			stringVec.push_back(columName);
			if (flag)
				floatVec.push_back(std::stof(columName));

		}
		flag = true;	
	this->m_table.push_back(stringVec);
	this->m_tableFloat.push_back(floatVec);
	stringVec.clear();
	floatVec.clear();
	}
}
void TimeSeries::insertLine(string newLine) {
	ifstream dataline(newLine);
	TimeSeries::insertColums(dataline);		
}
ifstream TimeSeries::openFile(const char* file) {
	ifstream data(file); // constructor that opening the file 
	if (!data.is_open()) throw runtime_error("could not open the file");
	return data;
}
vector<string> TimeSeries::getColumsInfo(int index) const{
	return m_table.at(index);
}
int TimeSeries::findIndexColoms(string nameFi) const {
		vector<string> names = getColumsName();
		int i = 0;
		for (string name : names) {
			if (name.compare(nameFi) == 0) {
				return i;
			}				
			i++;
		}
		return -1;
}
vector<float> TimeSeries::getColomnData(string fiName) const {
	int index = findIndexColoms(fiName);
	return m_tableTranspose.at(index);
}
vector<float> TimeSeries::getColomnData(int index) const {
	if (index > m_tableFloat.size()) throw out_of_range("no colomn match in the data, index too big");
	return m_tableTranspose.at(index);
}	
void TimeSeries::tableTranspose() {
	if (this->m_tableFloat.size() == 0)
		return;
	vector<vector<float>> trans_vec(this->m_table[0].size(), vector<float>());
	for (size_t i = 0; i < this->m_tableFloat.size(); i++)
    {
        for (size_t j = 0; j < this->m_tableFloat[i].size(); j++)
        {
            trans_vec[j].push_back(this->m_tableFloat[i][j]);
        }
    }
	this->m_tableTranspose = trans_vec;
}
int TimeSeries::getNumberOfFi() const{
	return m_tableTranspose.size();
}
int TimeSeries::getNumberOfMembers() const{	
	return m_table.size() - 1;
}

void TimeSeries::getArrOfCol(int index,float *arr) const {	
	vector<float> col = getColomnData(index);
	for (float num : col){
		*arr = num;
		arr++;
	}
}
string TimeSeries::valueFIAtTime(string fi,float time) const {
	vector<string> temp = getColumsInfo(time);
	int index = findIndexColoms(fi);
	return temp.at(index);
}
string TimeSeries::valueFIByIndex(int index) const{
	return m_table.at(0)[index];
}
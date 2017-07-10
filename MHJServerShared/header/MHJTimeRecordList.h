/*
 * MHJTimeRecordList.h
 *
 *  Created on: Aug 29, 2015
 *      Author: ruancongming
 */
#include "MHJFunctionTimeRecord.h"

#ifndef MHJTIMERECORDLIST_H_
#define MHJTIMERECORDLIST_H_

struct RecordList_Node{
	RecordList_Node *previous;
	MHJFunctionTimeRecorder *pRecoder;
	RecordList_Node *next;
};

struct RecordList_Header{
	RecordList_Node *first;
	int nodeNum;
};

class RecordList{
public:
	RecordList(RecordList_Header* header);
	~RecordList();

	int RecordList_GetLast(RecordList_Node *pNode);
	int RecordList_NodeAdd(RecordList_Node *toAdd);
	int RecordList_NodeDelet();
private:
	RecordList_Header* pheader;
};


#endif /* MHJTIMERECORDLIST_H_ */

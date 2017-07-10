/*
 * MHJTimeRecordList.cpp
 *
 *  Created on: Aug 29, 2015
 *      Author: ruancongming
 */

#include "MHJTimeRecordList.h"

RecordList::RecordList(RecordList_Header* ListHeader){
	pheader = ListHeader;
}

RecordList::~RecordList(){
	pheader = NULL;
}

int RecordList::RecordList_GetLast(RecordList_Node *pNode){
	int ret;
	if(pheader==NULL || pheader->nodeNum ==0){
		printf("list is empty");
		ret = 1;
	}else{
//		int i = pheader->nodeNum;
		RecordList_Node *p= pheader->first;
		while(p->next){
			p = p->next;
		}
		pNode = p;
		ret =0;
	}

	return ret;
}

int RecordList::RecordList_NodeAdd(RecordList_Node *toAdd){
	int ret;
	RecordList_Node *pLast;

	if(toAdd==NULL){
		ret = 1;
	}else{
		if(RecordList_GetLast(pLast)){
			ret = 1;
		}else{
			toAdd->previous = pLast;
			toAdd->next = NULL;
			pLast->next = toAdd;

			pheader->nodeNum++;
			ret = 0;
		}
	}
	return ret;
}

//default delet the last node
int RecordList::RecordList_NodeDelet(){
	int ret;
	RecordList_Node *pLast;

	if(RecordList_GetLast(pLast)){
		ret = 1;
	}else{
		RecordList_Node *previous = pLast->previous;
		previous->next = NULL;
		//TO DO  release memory alloc by Node, how to dealing with first and last code
//		free(pLast->pRecoder->MemAddr_star);
		pheader->nodeNum--;
		ret = 0;
	}

	return ret;
}

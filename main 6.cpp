//
//  main.cpp
//  Project
//
//  Created by Conghui Deng on 4/17/20.
//  Copyright © 2020 Conghui Deng. All rights reserved.
//

#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

int procUsed;
int currentTime;


class Scheduling{
public:
    class node{
    public:
        int jobId;
        int jobTime;
        int dependCount;
        node *next;
        node(int jI, int jT,int dC){
            jobId=jI;
            jobTime=jT;
            dependCount=dC;
        }
        void printNode(){
            
        }
    };
    class JOBS{
    public:
        int jobTime;
        int onWhichProc;
        int onOpen;
        int parentCount;
        int dependCount;
        JOBS(int jT,int oWP,int oP,int pC,int dC){
            jobTime=jT;
            onWhichProc=oWP;
            onOpen=oP;
            parentCount=pC;
            dependCount=dC;
        }
        JOBS(){
            
        }
    };
    class Proc{
    public:
        int doWhichJob;
        int timeRemain;
        
    };
    
    int numNodes;
    int numProcs;
    int procUsed;
    JOBS *jobAry;
    Proc *procAry;
    node *OPEN;
    int **adjMatrix;
    int *parentCountAry;
    int *dependCountAry;
    int *onGraphAry;
    int totalJobTimes;
    int **schedulingTable;
    
    void initialization(ifstream &in1,ifstream &in2){
        procUsed=0;
        currentTime=0;
        OPEN=new node(9999,9999,9999);
        in1>>numNodes;
        if(numProcs>numNodes){
            numProcs=numNodes;
        }
        adjMatrix=new int *[numNodes+1];
        for(int i=0;i<numNodes+1;i++){
            adjMatrix[i]=new int[numNodes+1];
            
        }
        for(int i=0;i<numNodes+1;i++)
           for(int j=0;j<numNodes+1;j++)
              adjMatrix[i][j]=0;
        
        jobAry=new JOBS [numNodes+1];
        procAry=new Proc [numProcs+1];
        for(int i=1;i<numProcs;i++){
            procAry[i].timeRemain=-1;
            procAry[i].doWhichJob=0;
        }
        schedulingTable=new int *[numProcs+1];
        loadMatrix(in1);
        computerParentCount();
        computeDependtCount();
        totalJobTimes=constructJobAry(in2);
        
        for(int i=0;i<numProcs+1;i++){
            schedulingTable[i]=new int[totalJobTimes+1];
        }
        
        for(int i=0;i<numProcs+1;i++)
            for(int j=0;j<totalJobTimes+1;j++)
                schedulingTable[i][j]=0;
        
        onGraphAry=new int[numNodes+1];
        for(int k=1;k<numNodes+1;k++)
            onGraphAry[k]=1;
        
    }
    void loadMatrix(ifstream &in){
        int i,j;
        while(in>>i>>j){
            adjMatrix[i][j]=1;
            
                    }
    }
    void computerParentCount(){
        parentCountAry=new int[numNodes+1];
        for(int k=1;k<numNodes+1;k++)
            parentCountAry[k]=0;
        
        for(int i=1;i<numNodes+1;i++){
            for(int j=1;j<numNodes+1;j++)
                parentCountAry[i]=adjMatrix[j][i]+parentCountAry[i];
            
        }
        
    }
    
    void computeDependtCount(){
        dependCountAry=new int [numNodes+1];
        for(int i=0;i<numNodes+1;i++){
            for(int j=0;j<numNodes+1;j++)
                dependCountAry[i]+=adjMatrix[i][j];
            
        }
    }
    int constructJobAry(ifstream &in){
        totalJobTimes=0;
        int nodeId,jobTime,totalTime=0;
        
        in>>nodeId;
        while(in>>nodeId){
           in>>jobTime;
           totalTime+=jobTime;
           // cout<<"jobId"<<nodeId<<endl;
            jobAry[nodeId].jobTime=jobTime;
            jobAry[nodeId].onWhichProc=0;
            jobAry[nodeId].onOpen=0;
            jobAry[nodeId].parentCount=parentCountAry[nodeId];
            jobAry[nodeId].dependCount=dependCountAry[nodeId];
            
        }
        return totalTime;
    }
    
    void loadOpen(){
        if(currentTime>0  && OPEN->dependCount!=9999){
        node *temp=OPEN;
        OPEN=new node(9999,9999,9999);
        OPEN->next=temp;
        }
        //cout<<"jobary3"<<parentCountAry[3]<<endl;
        int orphonNode=0,jobId,jobTime,j=1;
        while(j<=numNodes){
            orphonNode=findOrphan(j);
            
            if(orphonNode>0){
                jobId=orphonNode;
                jobTime=jobAry[jobId].jobTime;
                node *newNode=new node(jobId,jobTime,dependCountAry[jobId]);
                listInsert(newNode);
                jobAry[jobId].onOpen=1;
                //cout<<"jobid"<<jobId;
            }
            j++;
        }
    }
    
    void listInsert(node *newNode){
        if(OPEN==NULL)
            OPEN=newNode;
        else{
        node *Spot=findSpot(newNode);
        if(Spot!=NULL){
        newNode->next=Spot->next;
        Spot->next=newNode;
        }
        }
            
    }
    
    node *findSpot(node *newNode){
        
        node *temp=OPEN;
           while(temp->next!=NULL && temp->next->dependCount>newNode->dependCount){
               temp=temp->next;
           }
           return temp;
    }
    
    int findOrphan(int i){
       
            if(parentCountAry[i]<=0 && jobAry[i].onOpen==0 && jobAry[i].onWhichProc==0){
               // cout<<i;
                return i;
            }
           
        
            return -1;
    }
    
    void printList(ofstream &out){
        node *temp=OPEN;
        while(temp->next!=NULL){
            printNode(temp,out);
            temp=temp->next;
            out<<"->";
        }
        printNode(temp,out);
//        cout<<"->NULL"<<endl;
    }
    
    void printNode(node *Node,ofstream &outFile){

           if(Node->next==NULL){
               outFile<<"("<<Node->jobId<<", "<<Node->jobTime<<", "<<Node->dependCount<<", NULL)->NULL"<<endl;
           }
           else
           outFile<<"("<<Node->jobId<<", "<<Node->jobTime<<","<<Node->dependCount<<", "<<Node->next->jobId<<")->";
       }
    
    void loadProcAry(){
        int availProc;
        node *newJob;
        availProc=findProcessor();
        //node *temp=OPEN;
        
        while(availProc>0 && OPEN->next!=NULL && procUsed<numProcs){
            newJob=OPEN->next;
            if(jobAry[newJob->jobId].onOpen==1){
                procUsed++;
            procAry[availProc].doWhichJob=newJob->jobId;
            procAry[availProc].timeRemain=newJob->jobTime;
            putJobOnTable(availProc,newJob->jobId,newJob->jobTime);
            //cout<<endl<<newJob->jobTime;
            }
            availProc=findProcessor();
            OPEN=OPEN->next;
        }
//        if(jobAry[OPEN->jobId].onOpen<0 && OPEN->next!=NULL)
//             OPEN=OPEN->next;
//        else if(OPEN->jobId==9999 && OPEN->next==NULL)
//            OPEN=NULL;
//        else if(OPEN->jobId!=9999 && jobAry[OPEN->jobId].onOpen<0 && OPEN->next==NULL)
//            OPEN=NULL;
            
        //cout<<"out";
    }
    
    int findProcessor(){
        for(int i=1;i<numProcs+1;i++){
            if(procAry[i].timeRemain<=0){
              //  cout<<"f"<<i;
                return i;
            }
            //cout<<"i"<<i;
        }
        return -1;
    }
    
    void putJobOnTable(int availP, int jobId, int jobTime){
        int Time=currentTime;
        int EndTime=currentTime+jobTime;
        //cout<<endl<<jobId;
        while(Time<EndTime){
            schedulingTable[availP][Time]=jobId;
            Time++;
        }
         jobAry[jobId].onOpen=-1;
    }
    
    bool checkCycle(){
        if(OPEN->next==NULL && !graphIsEmpty() && checkProc())
            return true;
        
        return false;
    }
    
    bool graphIsEmpty(){
        for(int i=1;i<numNodes+1;i++){
           if(onGraphAry[i]==1)
               return false;
        }
        return true;
        
        
    }
    
    int findDoneProc(){
        int jobId;
        for(int i=1;i<numProcs+1;i++){
            if(procAry[i].doWhichJob>0 &&  procAry[i].timeRemain<=0){
                jobId=procAry[i].doWhichJob;
                procAry[i].doWhichJob=-1;
                procUsed--;
                //cout<<"done project"<<jobId<<endl;
                return jobId;
            }
        }
        return -1;
    }
    
    bool checkProc(){
        for(int i=1;i<numProcs+1;i++){
            if(procAry[i].timeRemain>0)
                return false;
        }
        return true;
    }
    
    void printSchedulingTable(ofstream &outFile1){
        outFile1<<"\n Table: "<<currentTime<<endl;
        outFile1<<"      ";
        int t=0;
        int p=1;
        while(t<currentTime+1){
            outFile1<<"-"<<t<<"--";
            t++;
            
        }
        t=0;
        while(p<numProcs+1){
            outFile1<<"\nP("<<p<<") | ";
            while(t<currentTime+1){
                if(schedulingTable[p][t]>0 && schedulingTable[p][t]<10)
                    outFile1<<schedulingTable[p][t]<<" | ";
                else if(schedulingTable[p][t]>=10){
                    outFile1<<schedulingTable[p][t]<<" |";
                }
                else
                    outFile1<<"- | ";
                t++;
            }
            t=0;
            p++;
            outFile1<<"\n    ------------------------------------------------------------- ";
        }
        }

    void updateProcTime(){
        for(int i=1;i<numProcs+1;i++){
            procAry[i].timeRemain--;
        }
        
    }
    
    void deleteFinishedNodes(){
        int jobId=findDoneProc();
             while(jobId>0){
                 onGraphAry[jobId]=0;
                 deleteEdge(jobId);
                 jobId=findDoneProc();
             }
    }
    
    void deleteEdge(int jID){
        for(int i=1;i<numNodes+1;i++){
            if(adjMatrix[jID][i]>0)
                parentCountAry[i]--;
            
        }
    }
};

Scheduling *s=new Scheduling;




int main(int argc, const char *argv[]){
    ifstream inFile1(argv[1]);
    ifstream inFile2(argv[2]);
    s->numProcs=*argv[3]-'0';
    //cout<<"numProcs="<<s->numProcs;

    ofstream outFile1(argv[4]);
    ofstream outFile2(argv[5]);
    s->initialization(inFile1,inFile2);
  
    while(!s->graphIsEmpty()){
        s->loadOpen();
        
        s->printList(outFile2);
        s->loadProcAry();
        bool hasCycle=s->checkCycle();
        if(hasCycle){
            cout<<"There is cycle in the graph!!!";
            outFile1<<"There is cycle in the graph!!!";
            break;
        }
        s->printSchedulingTable(outFile1);
        currentTime++;
        s->updateProcTime();
        s->deleteFinishedNodes();
        
    }
    s->printSchedulingTable(outFile1);
    inFile1.close();
    inFile2.close();
    outFile1.close();
    outFile2.close();
}



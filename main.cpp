#include "tsm.h"
#include "bellman.h"

using namespace std;

int main()
{
    ifstream fin("TSPtc.txt");
    int edgeList[100][3] ;
    int numEdges=80;
    int trave=1200;
    for(int i=0;i<numEdges;i++){
        fin>>edgeList[i][0]>>edgeList[i][1]>>edgeList[i][2];
    }
    fin.close();
    string output=Traveling(edgeList,numEdges,edgeList[0][0]);
    cout <<"TSP path:" << output << endl;
    cout << "Testing Bellman-Ford Algorithm" << endl;
    ifstream fin2("BFtc.txt");
    int edgeList2[100][3];
    int numEdges2=70;
    for(int i=0;i<numEdges2;i++){
        fin2>>edgeList2[i][0]>>edgeList2[i][1]>>edgeList2[i][2];
    }
    fin2.close();
    char start_vertices;
    char namelist[]="?b";

    for(int i=0;i<2;i++){
        start_vertices=namelist[i];
        int BFValue[10];
        int BFPrev[10];
        for(int k=0;k<10;k++){
            BFValue[k]=-1;
            BFPrev[k]=-1;
        }
        cout<<"#####"<<start_vertices<<endl;
        for(int j=0;j<6;j++){

            cout<<"step:"<<j+1<<endl;
            BF(edgeList2,numEdges2,start_vertices,BFValue,BFPrev);
            cout<<"Value";
            for(int k=0;k<10;k++){
                if(k%10==0) cout<<endl;
                cout<<BFValue[k]<<" ";
            }
            cout<<endl;
            cout<<"Prev";
            for(int k=0;k<10;k++){
                if(k%10==0) cout<<endl;
                cout<<BFPrev[k]<<" ";
            }
            cout<<endl;
        }
    }
    //test the bf_path function
    cout << "###Testing BF_Path function###" << endl;
    int edgeList3[70][3];
    int numEdges3=70;
    ifstream fin3("BFStringtc.txt");
    for(int i=0;i<numEdges3;i++){
        fin3>>edgeList3[i][0]>>edgeList3[i][1]>>edgeList3[i][2];
    }
    fin3.close();
    char start_vertices3;
    char end_vertices3;
    char startlist[]="?b";
    char endlist[]="5]";
    for(int i=0;i<2;i++){
        start_vertices3=startlist[i];
        end_vertices3=endlist[i];
        string out;
        cout<<"###"<<char(start_vertices3)<<"##"<<char(end_vertices3)<<"###"<<endl;
        out=BF_Path(edgeList3,numEdges3,start_vertices3,end_vertices3);
        cout<<out<<endl;
    }
    return 0;
}
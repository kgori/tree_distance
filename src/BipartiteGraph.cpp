#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif
#include "BipartiteGraph.h"

BipartiteGraph::BipartiteGraph(vector<deque<bool>>& IncidenceMatrix, const vector<double>& Aweight, const vector<double>& Bweight) : edge(IncidenceMatrix), nA(Aweight.size()), nB(Bweight.size()) {
    this->n = max(nA, nB);
    Avertex.reserve(nA);
    Bvertex.reserve(nB);
    for (auto dbl : Aweight)
        Avertex.push_back(Vertex(dbl));
    for (auto dbl : Bweight)
        Bvertex.push_back(Vertex(dbl));
}

vector<deque<bool>> BipartiteGraph::getIncidenceMatrix(vector<PhyloTreeEdge>& edges1, vector<PhyloTreeEdge>& edges2) {
    std::vector<std::deque<bool>> incidenceMatrix(edges1.size(), std::deque<bool>(edges2.size(), false));
    for (size_t i = 0; i < edges1.size(); i++) {
        for (size_t j = 0; j < edges2.size(); j++) {
            incidenceMatrix[i][j] = edges1[i].crosses(edges2[j]);
        }
    }
    return incidenceMatrix;
};

vector<vector<size_t>> BipartiteGraph::vertex_cover(const vector<size_t>& Aindex, const vector<size_t>& Bindex) {
    size_t nAVC = Aindex.size(), nBVC = Bindex.size(); //nAVC,nBVC=size of A and B
    double total = 0;
    vector<vector<double>> ABflow(nA, vector<double>(nB, 0.0));
    size_t i=0, j=0, k=0, AScanListSize=0, BScanListSize=0, augmentingPathEnd=0, Apathnode=0, Bpathnode=0;
    bool augmentingPathEndMINUS1 = true;
    vector<vector<size_t>> CD(4, vector<size_t>(this->n, 0)); //output: incidence vectors of vertex covers, CD[0]=Aside; CD[1]=Bside;
    vector<size_t> AScanList(nA, 0);
    vector<size_t> BScanList(nB, 0);

    /* First set normalized weights */
    total = 0;
    for (i = 0; i < nAVC; i++)
        total += Avertex[Aindex[i]].weight;
    for (i = 0; i < nAVC; i++)
        this->Avertex[Aindex[i]].residual = Avertex[Aindex[i]].weight / total;

    total = 0;
    for (j = 0; j < nBVC; j++)
        total += Bvertex[Bindex[j]].weight;
    for (j = 0; j < nBVC; j++)
        this->Bvertex[Bindex[j]].residual = Bvertex[Bindex[j]].weight / total;

    /* Now comes the flow algorithm
     * Flow on outside arcs are represented by Vertex[i].residual
     * Flow on inside arcs are represented by ABflow
     * Initialize ABflow to 0, start scanlist
     */

    total = 1; //flow augmentation in last stage
    while (total > 0) {
        //Scan Phase
        //Set labels
        total = 0;
        for (i = 0; i < nAVC; i++) {
            Avertex[Aindex[i]].label = -1;
            Avertex[Aindex[i]].pred = -1;
        }
        for (j = 0; j < nBVC; j++) {
            Bvertex[Bindex[j]].label = -1;
            Bvertex[Bindex[j]].pred = -1;
            Bvertex[j].label = -1;
        }
        AScanListSize = 0;
        for (i = 0; i < nAVC; i++) {
            if (Avertex[Aindex[i]].residual > 0) {
                Avertex[Aindex[i]].label = Avertex[Aindex[i]].residual;
                AScanList[AScanListSize] = Aindex[i];
                AScanListSize++;
            }
            else {
                Avertex[Aindex[i]].label = -1;
            }
        }
//        for (i = 0; i < nBVC; i++) {
//            Bvertex[i].label = -1;
//        }

        // scan for an augmenting path
        bool scanning = true;
        while (AScanListSize != 0 && scanning) {
            /* Scan the A side nodes*/
            BScanListSize = 0;
            for (i = 0; i < AScanListSize; i++) {
                for (j = 0; j < nBVC; j++) {
                    if (edge[AScanList[i]][Bindex[j]] && Bvertex[Bindex[j]].label == -1) {
                        Bvertex[Bindex[j]].label = Avertex[AScanList[i]].label;
                        Bvertex[Bindex[j]].pred = AScanList[i];
                        BScanList[BScanListSize] = Bindex[j];
                        BScanListSize++;
                    }
                }
            }
            /* Scan the B side nodes*/
            AScanListSize = 0;
            for (j = 0; j < BScanListSize; j++) {
                if (Bvertex[BScanList[j]].residual > 0) {
                    total = min(Bvertex[BScanList[j]].residual, Bvertex[BScanList[j]].label);
                    augmentingPathEnd = BScanList[j];
                    augmentingPathEndMINUS1 = false;
                    scanning = false;
                    break;
                } else {
                    for (i = 0; i < nAVC; i++) {
                        if (edge[Aindex[i]][BScanList[j]] && Avertex[Aindex[i]].label == -1 && ABflow[Aindex[i]][BScanList[j]] > 0) {
                            Avertex[Aindex[i]].label = min(Bvertex[BScanList[j]].label, ABflow[Aindex[i]][BScanList[j]]);
                            Avertex[Aindex[i]].pred = BScanList[j];
                            AScanList[AScanListSize] = Aindex[i];
                            AScanListSize++;
                        }
                    }
                }
            }
        }//scanning procedure

        if (total > 0) { // flow augmentation
            if (!augmentingPathEndMINUS1) {
                Bvertex[augmentingPathEnd].residual = Bvertex[augmentingPathEnd].residual - total;
                Bpathnode = augmentingPathEnd;
                Apathnode = Bvertex[Bpathnode].pred;

                ABflow[Apathnode][Bpathnode] = ABflow[Apathnode][Bpathnode] + total;
                while (Avertex[Apathnode].pred != -1) {
                    Bpathnode = Avertex[Apathnode].pred;
                    ABflow[Apathnode][Bpathnode] = ABflow[Apathnode][Bpathnode] - total;
                    Apathnode = Bvertex[Bpathnode].pred;
                    ABflow[Apathnode][Bpathnode] = ABflow[Apathnode][Bpathnode] + total;

                }
                Avertex[Apathnode].residual = Avertex[Apathnode].residual - total;
            }
        } else { //min vertex cover found, unlabeled A's, labeled B's
            k = 0;
            for (i = 0; i < nAVC; i++)
                if (Avertex[Aindex[i]].label == -1) {
                    CD[2][k] = Aindex[i];
                    k++;
                }
            CD[0][0] = k;
            k = 0;
            for (j = 0; j < nBVC; j++)
                if (Bvertex[Bindex[j]].label >= 0) {
                    CD[3][k] = Bindex[j];
                    k++;
                }
            CD[1][0] = k;
        }
    }//flow algorithm
    return CD;

} //vertex_cover;

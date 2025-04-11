#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>

using namespace std;

class GraphElement {
    protected:
        int id;
        double atr;
        string rule;

    public:
        GraphElement(int id): id(id), atr(0.0) {}
        virtual ~GraphElement() = default;

        int getId() const { return id; }
        double getAtr() const { return atr; }
        void setAtr(double value) { atr = value; }
        void setRule(const string& new_rule) { rule = new_rule; }
        const string& getRule() const { return rule; }

        virtual double computeAtr(const vector<GraphElement*>& elements, int numVertices) = 0;
};

class Vertex: public GraphElement {
    private:
        vector<int> incomingEdges;

    public:
        Vertex(int id): GraphElement(id) {}

        void addEdge(int edgeId) {
            incomingEdges.push_back(edgeId);
        }

        const vector<int>& getIncomingEdges() const {
            return incomingEdges;
        } 

        double computeAtr(const vector<GraphElement*>& elements, int numVertices) override {
            double oldValue = atr;
            istringstream iss(rule);
            string type;
            iss >> type;

            if (type == "min") {
                double minVal = numeric_limits<double>::max();
                for (int edgeIdx: incomingEdges) {
                    minVal = min(minVal, elements[edgeIdx]-> getAtr());
                }
                if (minVal != numeric_limits<double>::max()) {
                    atr = minVal;
                }
            } else if (type == "v") {
                int vertexId;
                iss >> vertexId;
                atr = elements[vertexId - 1]-> getAtr();
            } else if (type == "e") {
                int edgeId;
                iss >> edgeId;
                atr = elements[edgeId + numVertices - 1]-> getAtr();
            } else {
                try {
                    atr = stod(type);
                } catch(...) {
                    cerr << "Error rule vertex " << getId() << ": " << getRule() << endl;
                    atr = 0;
                }
            }
            return atr;
        }
};

class Edge: public GraphElement {
    private:
        int headId;
        int tailId;

    public:
        Edge(int id, int head, int tail): GraphElement(id), headId(head), tailId(tail) {}

        int getHead() const { return headId; }
        int getTail() const { return tailId; }
        

        double computeAtr(const vector<GraphElement*>& elements, int numVertices) override {
            double oldValue = atr;
            istringstream iss(rule);
            string type;
            iss >> type;

            if (type == "*") {
                double mulVal = elements[headId - 1]-> getAtr();
                Vertex* headVertex = dynamic_cast<Vertex*>(elements[headId - 1]);
                if (headVertex) {
                    for (int incEdgeId: headVertex->getIncomingEdges()){
                        mulVal *= elements[incEdgeId]-> getAtr();
                    }
                }
                atr = mulVal;
            } else if (type == "v") {
                int vertexId;
                iss >> vertexId;
                atr = elements[vertexId - 1]-> getAtr();
            } else if (type == "e") {
                int edgeId;
                iss >> edgeId;
                printf("edgeId");
                atr = elements[edgeId + numVertices - 1]-> getAtr();
            } else {
                try {
                    atr = stod(type);
                } catch(...) {
                    cerr << "Error rule edge " << getHead() << " " << getTail() << ": " << getRule() << endl;
                    atr = 0;
                }
            }
            return atr;
        }
};

class Graph {
    private:
        vector<GraphElement*> elements;
        int numVertices;
        int numEdges;

    public:
        Graph(int numV, int numE): numVertices(numV), numEdges(numE) {
            for (int i = 1; i <= numVertices; ++i) {
                elements.push_back(new Vertex(i));
            }
        }

        ~Graph() {
            for (auto elem: elements) {
                delete elem;
            }
        }

        int getNumVertices() {
            return numVertices;
        }

        int getNumEdges() {
            return numEdges;
        }

        void addEdge(int head, int tail) {
            int edgeId = elements.size() + 1;
            elements.push_back(new Edge(edgeId, head, tail));

            Vertex* tailVertex = dynamic_cast<Vertex*>(elements[tail - 1]);
            if (tailVertex) {
                tailVertex -> addEdge(elements.size() - 1);
            }
        }

        void setRule(int elemId, const string& rule) {
            elements[elemId - 1] -> setRule(rule);
        }

        bool computeAtr() {
            bool changedflag = false;
            for (auto elem: elements) {
                double oldValue = elem->getAtr();
                elem ->computeAtr(elements, numVertices);
                if (oldValue != elem->getAtr()) {
                    changedflag = true;
                }
            }
            return changedflag;
        }

        vector<double> getAllAttributes() {
            vector<double> res;
            for (auto elem: elements) {
                res.push_back(elem->getAtr());
            }
            return res;
        }
};



int main(int argc, char *argv[]) {
    // if (argc != 3) {
    //     cerr << "Use: " << argv[0] << " <input_file> <output_file>" << endl;
    // }

    //ifstream input(argv[1]);

    ifstream input("input.txt");
    if (!input) {
        cerr << "Error input file" << endl;
    } else {
        int numVertices, numEdges;
        string line;
        getline(input, line);
        istringstream iss(line);
        iss >> numVertices >> numEdges;

        getline(input, line);

        Graph graph(numVertices, numEdges);

        for (int i = 0; i < numEdges; ++i) {
            getline(input, line);
            istringstream iss(line);
            int head, tail;
            iss >> head >> tail;
            graph.addEdge(head, tail);
        }

        getline(input, line);

        for (int i = 0; i < numVertices; ++i) {
            getline(input, line);
            graph.setRule(i+1, line);
        }

        for (int i = 0; i < numEdges; ++i) {
            getline(input, line);
            graph.setRule(numVertices + i + 1, line);
        }

        while (graph.computeAtr()) {}

        //ofstream output(argv[2]);
        ofstream output("output.txt");
        if (!output) {
            cerr << "Error output file" << endl;
        } else {
            vector<double> res = graph.getAllAttributes();
            for (int i =0; i < res.size(); ++i) {
                output << res[i] << endl;
            }

            cout << "Success!" << endl;
        }
    }

    return 0;
}
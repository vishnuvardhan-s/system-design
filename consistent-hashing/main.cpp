#include <iostream>
#include <set>
#include <map>
using namespace std;

#define ull unsigned long long
ull M = 1ULL << 5;

void random_fill(vector<ull>& arr) {
    for (ull& a : arr) {
        a = rand() % M;
    }
}

class Hash {
public:
    ull hash(ull x) {
        x ^= (x << 13);
        x ^= (x >> 17);
        x ^= (x << 5);
        return x;
    }
};

class ConsistentHashing {
private:
    set<ull> server_positions;
    map<ull, ull> server_map;
    Hash h;
public:
    ConsistentHashing(Hash h, vector<ull> servers) {
        this->h = h;
        for (ull server : servers) {
            ull position = this->h.hash(server) % M;
            this->server_positions.insert(position);
            this->server_map[position] = server;
            cout << "Server " << server << " located at " << position << endl;
        }
    }

    void add_server(ull server) {
        ull position = this->h.hash(server) % M;
        this->server_positions.insert(position);
        this->server_map[position] = server;
    }

    void remove_server(ull server) {
        ull position = this->h.hash(server) % M;
        this->server_positions.erase(position);
        this->server_map.erase(server);
    }

    ull serve_request(ull request) {
        int position_size = this->server_positions.size();
        ull request_hash = this->h.hash(request) % M;
        cout << endl << "Request " << request << " is mapped to " << request_hash << endl;
        auto position = this->server_positions.lower_bound(request_hash);
        if (position == this->server_positions.end()) {
            return *this->server_positions.begin();
        }
        return this->server_map[*position];
    }
};

int main() {
    Hash h;
    vector<ull> servers = { 1, 5, 11, 20 };
    vector<ull> requests = { 3, 8, 15, 25 };
    ConsistentHashing c(h, servers);
    for (ull& request : requests) {
        ull server = c.serve_request(request);
        cout << "Request " << request << " served with " << server << endl;
    }
    c.remove_server(20);
    for (ull& request : requests) {
        ull server = c.serve_request(request);
        cout << "Request " << request << " served with " << server << endl;
    }
    c.add_server(25);
    for (ull& request : requests) {
        ull server = c.serve_request(request);
        cout << "Request " << request << " served with " << server << endl;
    }
}
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
    // log(M) number of hashes can be defined - K
    ull hash(ull x) {
        x ^= (x << 13);
        x ^= (x >> 17);
        x ^= (x << 5);
        return x;
    }

    ull hash1(ull x) {
        x ^= (x << 7);
        x ^= (x >> 3);
        x ^= (x << 5);
        return x;
    }

    ull hash2(ull x) {
        x ^= (x << 23);
        x ^= (x >> 29);
        x ^= (x << 5);
        return x;
    }

    ull hash3(ull x) {
        x ^= (x << 31);
        x ^= (x >> 37);
        x ^= (x << 5);
        return x;
    }

    vector<function<ull(ull)>> get_hashes() {
        return vector<function<ull(ull)>>{
            bind(&Hash::hash1, this, placeholders::_1),
                bind(&Hash::hash2, this, placeholders::_1),
                bind(&Hash::hash3, this, placeholders::_1)
        };
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
            typedef void (Hash::* h)();
            for (function<ull(ull)> hash : this->h.get_hashes()) {
                ull position = hash(server) % M;
                this->server_positions.insert(position);
                // if collision happens, we simply override with the new server
                this->server_map[position] = server;
                cout << "Server " << server << " located at " << position << endl;
            }
        }
    }

    void add_server(ull server) {
        for (function<ull(ull)> hash : this->h.get_hashes()) {
            ull position = hash(server) % M;
            this->server_positions.insert(position);
            this->server_map[position] = server;
            cout << "Server " << server << " located at " << position << endl;
        }
    }

    void remove_server(ull server) {
        for (function<ull(ull)> hash : this->h.get_hashes()) {
            ull position = hash(server) % M;
            this->server_positions.erase(position);
            this->server_map.erase(position);
        }
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
    c.add_server(30);
    for (ull& request : requests) {
        ull server = c.serve_request(request);
        cout << "Request " << request << " served with " << server << endl;
    }
}
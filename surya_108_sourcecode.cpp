#include<bits/stdc++.h>
using namespace std;
int N = 1000;
//starting customer id

int D = 100;

const int lpg_price = 1000;
const int lpg_id = 101;

const int regulator_price = 500;
const int regulator_id = 102;

const int lighter_price = 200;
const int lighter_id = 103;

const int n = 6;
float adj[n][n] = {
    {0, 17, 13, 7, 7.8, 26},
    {17, 0, 8, 4, 8, 6},
    {13, 8, 0, 3, 5, 9},
    {7, 4, 3, 0, 6.8, 10},
    {7, 8, 5, 6.8, 0, 15.5},
    {26, 6, 9, 10, 15.5, 0},
};

int get_index(string& location) {
    if (location == "omr") return 0;
    else if (location == "vadapalani") return 1;
    else if (location == "velachery") return 2;
    else if (location == "guindy") return 3;
    else if (location == "beasant_nagar") return 4;
    else if (location == "porur") return 5;
    else return 7;
}

int generate_customer_id() {
    return N++;
}

int generate_distributor_id() {
    return D++;
}

class customer_details {
public:
    int phone_no;
    string name, address;
    int customer_id;
    customer_details(int no, int id, string &n, string &add) {
        address = add;
        name = n;
        customer_id = id;
        phone_no = no;
    }
    customer_details(customer_details &cd) {
        phone_no = cd.phone_no;
        name = cd.name;
        address = cd.address;
        customer_id = cd.customer_id;
    }
    void display(){
        cout<<"Phone number: "<<phone_no<<endl;
        cout<<"Name: "<<name<<endl;
        cout<<"Address: "<<address<<endl;
        cout<<"Customer ID: "<<customer_id<<endl;
    }

};

class product_details {
public:
    string name;
    int price, product_id, stock;
    product_details(string &n, int p, int id, int s) {
        name = n;
        price = p;
        product_id = id;
        stock = s;
    }
};

class distributor_details {
public:
    string name, zone;
    int phone_no, distributor_id;
    vector<product_details>products;
    //0->LPG 1->regulator 2->lighter
    distributor_details(int phone, int id, string &z, string &n, int lpg_stock, int regulator_stock, int lighter_stock) {
        phone_no = phone;
        distributor_id = id;
        zone = z;
        name = n;
        string p_name = "LPG";
        products.push_back(product_details(p_name, lpg_price, lpg_id, lpg_stock));
        p_name = "regulator";
        products.push_back(product_details(p_name, regulator_price, regulator_id, regulator_stock));
        p_name = "lighter";
        products.push_back(product_details(p_name, lighter_price, lighter_id, lighter_stock));
    }
};

//end of classes

//avl tree construction

struct node {
public:
    node * left;
    node * right;
    customer_details cd;

}*root;

int height(node * root) {
    if (!root) return 0;
    return 1 + max(height(root->left), height(root->right));
}

int balance_factor(node * root) {
    return height(root->left) - height(root->right);
}

node * right_rotate(node * parent) {
    node * child = parent->left;
    parent->left = child->right;
    child->right = parent;
    return child;
}

node * left_rotate(node * parent) {
    node * child = parent->right;
    parent->right = child->left;
    child->left = parent;
    return child;
}

node * balance(node * root) {
    int bf = balance_factor(root);
    if (bf < -1) {
        if (balance_factor(root->right) <= 0) {
            //right/right case
            return left_rotate(root);
        }
        else {
            //right/left case
            root = right_rotate(root->right);
            return left_rotate(root);
        }
    }
    else if (bf > 1) {
        if (balance_factor(root->left) >= 0) {
            //left left case
            return right_rotate(root);
        }
        else {
            root = left_rotate(root->left);
            return right_rotate(root);
        }
    }
    return root;
}

node * find_min(node * root) {
    node * temp = root;
    while (temp->left) {
        temp = temp->left;
    }
    return temp;
}

node * del(node * root, int target) {
    if (!root) return root;
    if (root->cd.customer_id > target) {
        root->left = del(root->left, target);
    }
    else if (root->cd.customer_id < target) {
        root->right = del(root->right, target);
    }
    else {
        if (!root->left and !root->right) {
            root = NULL;
        }
        else if (!root->left or !root->right) {
            node * temp = (root->left ? root->left : root->right);
            *root = *temp;
            free(temp);
        }
        else {
            node * temp = find_min(root->right);
            root->cd.customer_id = temp->cd.customer_id;
            root->right = del(root->right, temp->cd.customer_id);

        }
    }
    if (!root) return NULL;
    return balance(root);
}

node * insert(node * root, customer_details new_node) {
    if (!root) {
        //cout<<new_node.customer_id<<endl;
        root = (node *)(malloc(sizeof(node)));
        root->cd = new_node;
        root->left = root->right = NULL;
        root = balance(root);
    }
    else if (root->cd.customer_id > new_node.customer_id) {
        root->left = insert(root->left, new_node);
        root = balance(root);
    }
    else if (root->cd.customer_id < new_node.customer_id) {
        root->right = insert(root->right, new_node);
        root = balance(root);
    }
    return root;
}

bool search(node * root, int data) {
    if (!root) {
        return false;
    }
    if (root->cd.customer_id == data) {
        return true;
    }
    else if (root->cd.customer_id > data) {
        return search(root->left, data);
    }
    else if (root->cd.customer_id < data) {
        return search(root->right, data);
    }
    return true;

}

void preorder(node * root) {
    if (root) {
        cout << root->cd.customer_id << " ";
        preorder(root->left);
        preorder(root->right);
    }
}

void calc_distance() {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                adj[i][j] = min(adj[i][j], adj[i][k] + adj[k][j]);
            }
        }
    }
}

vector<distributor_details>d;

node* get_customer_details(node * root, int cid) {
    if (root->cd.customer_id > cid) return get_customer_details(root->left, cid);
    else if (root->cd.customer_id < cid) return get_customer_details(root->right, cid);
    else return root;
}
void place_order(int cid) {
    if (d.empty()) {
        cout << "No distributors available :(\n";
        return ;
    }
    node * info = get_customer_details(root, cid);
    int regulators = 0, lighters = 0;
    cout << "Do you want any other products (regulators/lighters)\n";
    cout << "YES/NO Enter your choice: ";
    string ch;
    cin >> ch;
    if (ch == "YES" or ch == "Yes" or ch == "yes") {
        cout << "Enter the amount of regulators needed: ";
        cin >> regulators;
        cout << "Enter the amount of lighters needed: ";
        cin >> lighters;
    }
    string location;
    location = info->cd.address;
    map<int, int>pos;
    set<pair<float, int>>possible;
    //each pair {distance,distributor id}

    for (int i = 0; i < d.size(); i++) {
        int lpg_stock = d[i].products[0].stock, regulator_stock = d[i].products[1].stock, lighter_stock = d[i].products[2].stock;
        if (regulator_stock > regulators and lighter_stock > lighters and lpg_stock > 0) {
            int distance = adj[get_index(location)][get_index(d[i].zone)];
            possible.emplace(distance, d[i].distributor_id);
            pos[d[i].distributor_id] = i;
        }
    }
    if (possible.empty()) {
        cout << "No distributors availale :(\n";
        return;
    }
    //sort(possible.begin(), possible.end());
    int assigned = (*possible.begin()).second;
    int assigned_index = pos[assigned];
    system("clear");
    cout << "Order placed Successfully\n\n";
    cout<<"Generated Bill\n\n";
    cout << "LPG cylinder price: 1 X " << d[assigned_index].products[0].price << " = " << d[assigned_index].products[0].price << endl << endl;
    cout << "Regulator price :" << regulators << " X " << d[assigned_index].products[1].price << " = " << regulators*d[assigned_index].products[1].price;
    cout << endl << endl;
    cout << "Lighter price :" << lighters << " X " << d[assigned_index].products[2].price << " = " << lighters*d[assigned_index].products[2].price;

    cout<<"\n\nTotal amount generated: "<<d[assigned_index].products[0].price+regulators*d[assigned_index].products[1].price+lighters*d[assigned_index].products[2].price<<endl<<endl;
    //cout << endl << endl;
    cout << "Details of the distributor: \n";
    cout << "Distributor Name: " << d[assigned_index].name << endl;
    cout << "Distributor location: " << d[assigned_index].zone << endl;
    cout << "Distributor ID: " << assigned << endl;
    cout << "Distance from distributor: " << (*possible.begin()).first << endl<<endl;
    d[assigned_index].products[0].stock--;
    d[assigned_index].products[1].stock -= regulators;
    d[assigned_index].products[2].stock -= lighters;


}
void print_locations(){
    cout<<"List of servicable locations: \n\n";
    cout<<"OMR\n";
    cout<<"Guindy\n";
    cout<<"Velachery\n";
    cout<<"vadapalani\n";
    cout<<"Beasant_Nagar\n";
    cout<<"Porur\n\n";
}

int main() {
    root = NULL;
    int ok = 1;
    calc_distance();
    while (ok) {
        system("clear");
        cout << "1. Customer\n";
        cout << "2. Distributor\n";
        int n;
        cout << "Enter your choice: ";
        cin >> n;
        if (n == 1) {
           system("clear");
            cout<<"Customer options: \n\n";
            cout << "1. Sign in\n";
            cout << "2. Sign up\n";
            cout << "Enter your choice: ";
            cin >> n;
            if (n == 1) {
                int cid;
                system("clear");
                cout<<"Customer sign in options: \n\n";
                cout << "Enter the customer id: ";
                cin >> cid;
                if (search(root, cid)) {
                    system("clear");
                    cout << "Sucessfully signed in\n";
                    cout << "1. Delete details\n";
                    cout << "2. place order\n";
                    cout << "3. Exit\n";
                    int ch;
                    cin >> ch;
                    if (ch == 1) {
                        root = del(root, cid);
                        cout << "Successfully deleted the customer details\n";
                    }
                    else if (ch == 2) {
                        place_order(cid);
                    }
                    else if (ch != 3) {
                        cout << "Wrong choice :(\n";
                    }
                }
                else {
                    cout << "Customer id " << cid << " not found\n";
                }
            }
            else if (n == 2) {
                system("clear");
                cout<<"Customemr sign up options: \n\n";
                print_locations();
                int phone;
                cout << "Enter the phone number: ";
                cin >> phone ;
                string name, a;
                cout << "Enter the name: ";
                cin >> name;
                cout << "Enter the location: ";
                cin >> a;
                transform(a.begin(), a.end(), a.begin(), ::tolower);
                int cid = generate_customer_id();
                customer_details temp(phone, cid, name, a);
                root = insert(root, temp);
                system("clear");
                cout << "Successfully inserted details\n\n";
                //system("CLS");
                temp.display();
                //system("CLS");
                cout << "\n1. place order\n";
                cout << "2. Exit\n";
                int ch;
                cout << "Enter your choice: ";
                cin >> ch;
                system("clear");
                if (ch == 1)
                {
                    place_order(cid);
                }
                else if (ch != 2) {
                    cout << "Wrong choice :(\n";
                }
                //preorder(root);

            }
            //for exiting or continuing
            string ch;
            ok = 0;
            cout << "Do you want to continue (YES / NO): ";
            cin >> ch;
            if (ch == "YES" or ch == "Yes" or ch == "yes" or ch == "y" or ch == "Y") {
                ok = 1;
                n = 1;
            }

        }
        else if (n == 2) {
            
            system("clear");
            cout<<"Distributor options: \n\n";
            cout << "1.Add distributor details\n";
            cout << "2.Exit\n";
            int c;
            cout << "Enter your choice: ";
            cin >> c;
            if (c == 1) {
                string name, zone;
                int phone_no;
                int r_stock, lpg_stock, lighter_stock;
                system("clear");
                print_locations();
                cout << "Details of distributor: \n";
                cout << "Enter the name of the distributor: ";
                cin >> name;
                cout << "Enter the zone of the distributor: ";
                cin >> zone;
                transform(zone.begin(), zone.end(), zone.begin(), ::tolower);
                cout << "Enter the phone number of the distributor: ";
                cin >> phone_no;
                cout << "Stock details: \n";
                cout << "Enter the regulator stock, LPG cylinders stock and lighters stock: ";
                cin >> r_stock >> lpg_stock >> lighter_stock;
                int D_id = generate_distributor_id();
                d.push_back(distributor_details(phone_no, D_id, zone, name, lpg_stock, r_stock, lighter_stock));
                system("clear");
                cout << "Successfully inserted details\n";
                //display(D_id);

            }
            string ch;
            ok = 0;
            cout << "Do you want to continue (YES / NO): ";
            cin >> ch;
            if (ch == "YES" or ch == "Yes" or ch == "yes" or ch == "y" or ch == "Y") {
                ok = 1;
                n = 1;
            }
        }
        else {
            cout << "Wrong Choice:( \n";
            ok=0;
        }

    }
    system("clear");
    cout<<"\nPreorder traversal of the AVL tree consisting the customer nodes\n";
    preorder(root);
    return 0;
}

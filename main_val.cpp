/* #region   */
#include <bits/stdc++.h>
using namespace std;
#define gc getchar_unlocked
#define fo(i, n) for (auto i = 0; i < n; i++)
#define Fo(i, k, n) for (auto i = k; k < n ? i < n : i > n; k < n ? i += 1 : i -= 1)
#define ll long long
#define si(x) scanf("%d", &x)
#define sl(x) scanf("%lld", &x)
#define ss(s) scanf("%s", s)
#define pi(x) printf("%d\n", x)
#define pl(x) printf("%lld\n", x)
#define ps(s) printf("%s\n", s)
#define deb(x) cout << #x << "=" << x << endl
#define deb2(x, y) cout << #x << "=" << x << "," << #y << "=" << y << endl
#define pb push_back
#define mp make_pair
#define F first
#define S second
#define all(x) x.begin(), x.end()
#define clr(x) memset(x, 0, sizeof(x))
#define sortall(x) sort(all(x))
#define tr(it, a) for (auto it = a.begin(); it != a.end(); it++)
#define PI 3.1415926535897932384626
typedef pair<int, int> pii;
typedef pair<ll, ll> pl;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<ll> vll;
typedef vector<pii> vpii;
typedef vector<pl> vpl;
typedef vector<pll> vpll;
typedef vector<vi> vvi;
typedef vector<vl> vvl;
typedef vector<vll> vvll;
#define lb lower_bound
#define ub upper_bound
#define PRESENT -1
#define FULL 0
#define SUCCESS 1
/* #endregion */

ll ctr = 1;

/* #region  bucket class definitions */
class bucket
{
private:
    int local_depth, size; // size is always size of bucket
    set<int> values;
    int creation_time;

public:
    bucket(int, int);
    int insert(int key);
    set<int> allvals();
    void clear();
    void remove(int key);
    int getLocalDepth();
    int getNumKeys();
    int incLocalDepth();
    int getCreationTime();
    void search(int key, string id);
    ~bucket();
};
/* #endregion */

/* #region  directory class definitions */
class directory
{
private:
    int global_depth, size; // size is always size of bucket
    vector<bucket *> ptr_to_buckets;
    int hash_func(int);
    void split(int bucket_no);
    void dir_dbl();
    bool personalcomparator(bucket *a, bucket *b);

public:
    directory(int depth, int size_assign);
    void insert(int key, bool);
    void delete_value(int key);
    void search(int key);
    string bucket_rep(int n);
    int mirrorIndex(int bucket_num, int depth);
    void statusUpdate();
    // void remove(int key, int mode);
    // void update(int key, string value);

    // void display(bool duplicates);
    ~directory();
};
/* #endregion */

/* #region  bucket functions */

bucket::bucket(int depth, int size_assign)
{
    local_depth = depth;
    size = size_assign;
    creation_time = ctr++;
}

int bucket::getCreationTime()
{
    return creation_time;
}

int bucket::getLocalDepth()
{
    return local_depth;
}

int bucket::incLocalDepth()
{
    local_depth++;
    return local_depth;
}

int bucket::insert(int key)
{
    if (values.find(key) != values.end())
        return PRESENT;
    if (values.size() == size)
        return FULL;
    values.insert(key);
    return SUCCESS;
}

set<int> bucket::allvals()
{
    set<int> ret(all(values));
    return ret;
}

void bucket::clear()
{
    values.clear();
    // return SUCCESS;
}

void bucket::remove(int key)
{
    if (values.find(key) == values.end())
    { // cout << "key " << key << " does not exist\n";
    }
    else
    {
        // cout << "key removed successfully.\n";
        // cout << "old size:" << values.size() << "\n";
        values.erase(values.find(key));
        // cout << "new size:" << values.size() << "\n";
    }
}

void bucket::search(int key, string buck)
{
    if (values.find(key) == values.end())
    {
        // cout << "key " << key << " does not exist\n";
    }
    else
    {
        // cout << "key found successfully in bucket: "<< buck << "\n";
    }
}

int bucket::getNumKeys()
{
    return values.size();
}

bucket::~bucket()
{
}
/* #endregion */

/* #region  directory functions */
directory::directory(int depth, int size_assign)
{
    global_depth = depth;
    size = size_assign;
    for (auto i = 0; i < (1 << global_depth); i++)
    {
        ptr_to_buckets.push_back(new bucket(global_depth, size));
    }
}

bool directory::personalcomparator(bucket *a, bucket *b)
{
    return (a->getCreationTime()) < (b->getCreationTime());
}
void directory::statusUpdate()
{
    cout /*<< "global depth is:" */ << global_depth << "\n";
    int numBuck = 0;
    auto copy_of_dir_ptrs = ptr_to_buckets;
    sort(
        all(copy_of_dir_ptrs),
        [](bucket *a, bucket *b)
        {
            return (a->getCreationTime() >
                    b->getCreationTime());
        });
    copy_of_dir_ptrs.erase(unique(all(copy_of_dir_ptrs)), copy_of_dir_ptrs.end());
    for (auto i : copy_of_dir_ptrs)
        if (i->getNumKeys() != 0)
            numBuck++;
    cout << numBuck << "\n";
    for (auto i : copy_of_dir_ptrs)
        if (i->getNumKeys() != 0)
            cout << i->getNumKeys() << " " << i->getLocalDepth() << "\n";
}
int directory::hash_func(int val)
{
    return (val & ((1 << global_depth) - 1));
}

string directory::bucket_rep(int bucket_num)
{
    int lD = ptr_to_buckets[bucket_num]->getLocalDepth();
    string ans = bitset<32>(bucket_num).to_string();
    reverse(all(ans));
    string ret;
    for (int i = 0; i < lD; i++)
    {
        ret += ans[i];
    }
    reverse(all(ret));
    return ret;
}

int directory::mirrorIndex(int bucket_num, int depth)
{
    return (bucket_num ^ (1 << (depth - 1)));
}

void directory::insert(int key, bool stat_reins = false)
{
    int bucket_num = hash_func(key);
    int status = ptr_to_buckets[bucket_num]->insert(key);
    if (status == PRESENT)
    {
        // cout << "Key " << key << " already exists in "
        //      << bucket_rep(bucket_num) << "\n";
    }
    else if (status == FULL)
    {
        // cout << "Split Occured - bucket: " << bucket_rep(bucket_num) << " reached capacity \n";
        split(bucket_num);
        insert(key);
    }
    else // SUCCESS
    {
        if (stat_reins)
        {
            // cout << "Reinserted key " << key << " in bucket "
            //      << bucket_rep(bucket_num) << "\n";
        }
        else
        {
            // cout << "Inserted key " << key << " in bucket "
            //  << bucket_rep(bucket_num) << "\n";
        }
    }
}

void directory::dir_dbl()
{
    for (int i = 0; i < 1 << (global_depth); i++)
        ptr_to_buckets.push_back(ptr_to_buckets[i]);
    global_depth++;
}

void directory::split(int bucket_num)
{
    int newLD = ptr_to_buckets[bucket_num]->incLocalDepth();
    if (newLD > global_depth)
        dir_dbl();
    int newBucket_num = mirrorIndex(bucket_num, newLD);
    ptr_to_buckets[newBucket_num] = new bucket(newLD, size);
    auto buffer_for_reinserting = ptr_to_buckets[bucket_num]->allvals();
    ptr_to_buckets[bucket_num]->clear();
    for (auto i : buffer_for_reinserting)
        insert(i, 1);
}

void directory::search(int key)
{
    int bucket_no = hash_func(key);
    // cout << "Searching key " << key << " in bucket " << bucket_rep(bucket_no) << "\n";
    ptr_to_buckets[bucket_no]->search(key, bucket_rep(bucket_no));
}

void directory::delete_value(int key)
{
    int bucket_num = hash_func(key);
    ptr_to_buckets[bucket_num]->remove(key);
}
directory::~directory()
{
}
/* #endregion */

int main()
{
    int gl_depth_inp, bucket_size;
    cin >> gl_depth_inp >> bucket_size;
    directory dir(gl_depth_inp, bucket_size);
    for (int i = 0; i != -1;)
    {
        int option;
        cin >> option;
        if (option == 2)
        {
            int val;
            cin >> val;
            dir.insert(val);
        }
        else if (option == 3)
        {
            int val;
            cin >> val;
            dir.search(val);
        }
        else if (option == 4)
        {
            int val;
            cin >> val;
            dir.delete_value(val);
        }
        else if (option == 5)
        {
            dir.statusUpdate();
        }
        else
        {
            i = -1;
        }
    }
    // dir.insert(0);
    // dir.insert(2);
    // dir.insert(4);
    // dir.insert(1);
    // dir.insert(6);
    // dir.insert(8);
    // for (int i = 0; i < 50; i += 2)
    //     dir.insert(i);
}
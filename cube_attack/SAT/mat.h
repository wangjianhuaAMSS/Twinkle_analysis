#ifndef _MAT_H_
#define _MAT_H_




template<typename T>
class Mat{

public:
    typedef T value_type;

    //construct
    Mat();
    Mat(size_t i, size_t j);
    //copy constructor
    Mat(const Mat& m);

    //1-dim array constructor
    Mat(const T arr[], size_t i, size_t j);

    //copy assignment
    Mat& operator=(const Mat&m);

    // +=
    Mat& operator+=(const Mat& m);

    // -=
    Mat& operator-=(const Mat& m);

    // *=
    Mat& operator*=(const Mat& m);

    //move constructor
    Mat( Mat&& m);

    //move assignment
    Mat& operator=( Mat&& m);

    //destructor
    ~Mat();

    //access element value
    T& operator()(size_t i, size_t j);
    const T& operator()(size_t i, size_t j) const;

    //get row and col number
    const size_t rows() const{ return vdata.size(); }
    const size_t cols() const{ 
        if (vdata.empty()) return 0;
        else return vdata[0].size(); 
    }

    //resize
    void resize(size_t nr, size_t nc);

    //print mat
    void CoutMat(ostream& os) const;
    void CinMat(istream& is);

private:

    std::vector<std::vector<T>> vdata;
};



template<typename T>
ostream& operator << (ostream &os, const Mat<T> &m){
    m.CoutMat(os);
    return os;
}

template<typename T>
istream& operator >> (istream &is, Mat<T>&m){
    m.CinMat(is);
    return is;
}


// +
template<typename T>
const Mat<T> operator+(const Mat<T>& m1, const Mat<T>& m2){
    Mat<T> t(m1);
    t += m2;
    return t;
}

// -
template<typename T>
const Mat<T> operator-(const Mat<T>& m1, const Mat<T>& m2){
    Mat<T> t(m1);
    t -= m2;
    return t;
}

// *
template<typename T>
const Mat<T> operator*(const Mat<T>& m1, const Mat<T>& m2){
    Mat<T> t(m1);
    t *= m2;
    return t;
}

//transpose
template<typename T>
const Mat<T> transpose(const Mat<T>& m){
    const size_t row = m.rows();
    const size_t col = m.cols();
    Mat<T> m1(col, row); 
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            m1(i, j) = m(j, i);
        }
    }
    return(m1);  
}

// print mat
template<typename T>
void Mat<T>::CoutMat(ostream& os) const
{
    if (vdata.empty()) return;
    for (size_t i = 0; i < vdata.size(); i++){
        for (size_t j = 0; j < vdata[0].size(); j++){
            os << vdata[i][j] << " ";
        }
        os << endl;
    }
    os << endl;
}

template<typename T>
void Mat<T>::CinMat(istream& is)
{
    if (vdata.empty()) return;
    for (size_t i = 0; i < vdata.size(); i++){
        for (size_t j = 0; j < vdata[0].size(); j++){
            is >> vdata[i][j];
        }
    }
}

//construct
template<typename T>
Mat<T>::Mat(){
    //cout << "default constructor" << endl;
    vdata.clear();
}

template<typename T>
Mat<T>::Mat(size_t i, size_t j){
    vector<vector<T>> tdata(i, vector<T>(j, 0));
    vdata = move(tdata);
}

//copy constructor
template<typename T>
Mat<T>::Mat(const Mat<T>& m){
    //cout << "copy constructor" << endl;
    vdata.assign(m.vdata.cbegin(), m.vdata.cend());
}

//1-dim array constructor
template<typename T>
Mat<T>::Mat(const T arr[], size_t i, size_t j) {
    vector<vector<T>> tdata(i, vector<T>(j, 0));
    for (size_t i0 = 0; i0 < i; i0++) {
        for (size_t j0 = 0; j0 < j; j0++) {
            tdata[i0][j0] = arr[i0 * j + j0];
        }
    }
    vdata = move(tdata);
}

//copy assignment
template<typename T>
Mat<T>& Mat<T>::operator=(const Mat<T>& m){
    //cout << "copy assignment" << endl;
    if (this != &m){
        vdata.assign(m.vdata.cbegin(), m.vdata.cend());
    }
    return *this;
}

//move constructor
template<typename T>
Mat<T>::Mat( Mat<T>&& m ){
    //cout << "move constructor" << endl;
    vdata = std::move(m.vdata);
}

//move assignment
template<typename T>
Mat<T>& Mat<T>::operator=(Mat<T>&& m){
    //cout << "move assignment" << endl;
    if (this != &m){
        vdata.clear();
        vdata = std::move(m.vdata);
    }
    return *this;
}

//destructor
template<typename T>
Mat<T>::~Mat(){
    vdata.clear();
}

//access element value
template<typename T>
inline T& Mat<T>::operator()(size_t i, size_t j){
    assert(!vdata.empty());
    assert(i >= 0 && j >= 0 && i < vdata.size() && j < vdata[0].size());
    return vdata[i][j];
}
template<typename T>
inline const T& Mat<T>::operator()(size_t i, size_t j) const{
    assert(!vdata.empty());
    assert(i >= 0 && j >= 0 && i < vdata.size() && j < vdata[0].size());
    return vdata[i][j];
}

// +=
template<typename T>
Mat<T>& Mat<T>::operator+=(const Mat<T>& m){
    if (vdata.empty() || m.vdata.empty()) return *this;

    const size_t row = vdata.size();
    const size_t col = vdata[0].size();
    const size_t mrow = m.vdata.size();
    const size_t mcol = m.vdata[0].size();

    if (row == mrow && col == mcol){
        for (size_t i = 0; i < row; i++)
        for (size_t j = 0; j < col; j++)
            vdata[i][j] += m.vdata[i][j];
    }
    else{
        std::cerr << "mat must be the same size." << std::endl;
    }

    return *this;
}

// -=
template<typename T>
Mat<T>& Mat<T>::operator-=(const Mat<T>& m){
    if (vdata.empty() || m.vdata.empty()) return *this;

    const size_t row = vdata.size();
    const size_t col = vdata[0].size();
    const size_t mrow = m.vdata.size();
    const size_t mcol = m.vdata[0].size();

    if (row == mrow && col == mcol){
        for (size_t i = 0; i < row; i++)
        for (size_t j = 0; j < col; j++)
            vdata[i][j] -= m.vdata[i][j];
    }
    else{
        std::cerr << "mat must be the same size." << std::endl;
    }

    return *this;
}




//resize
template<typename T>
void Mat<T>::resize(size_t nr, size_t nc){
    vdata.resize(nr);
    for (size_t i = 0; i < nr; i++){
        vdata[i].resize(nc);
    }
}


// *=
template<typename T>
Mat<T>& Mat<T>::operator*=(const Mat<T>& m){
    if (vdata.empty() || m.vdata.empty()) return *this;

    const size_t row = vdata.size();
    const size_t col = vdata[0].size();
    const size_t mrow = m.vdata.size();
    const size_t mcol = m.vdata[0].size();

    if (col == mrow){
        Mat<T> mm(row, mcol);
        for (size_t i = 0; i < row; i++) {
            for (size_t j = 0; j < mcol; j++) {
                mm.vdata[i][j] = 0;
                for (int k = 0; k < col; k++) {
                    mm.vdata[i][j] += vdata[i][k] * m.vdata[k][j];
                } 
            }
        } 
        vdata = move(mm.vdata);   
    }
    else{
        cerr << "mat's col must be equal to the other's row." << std::endl;
    }

    return *this;
}


//int matrix

//zero matrix
const Mat<int> zero_mat(size_t rows, size_t cols){
    Mat<int> t(rows, cols);
    return t;
}

//identity matrix
const Mat<int> identity_mat(size_t rows){
    Mat<int> t(rows, rows);
    for (size_t i = 0; i < rows; i++) {
        t(i, i) = 1;
    }
    return t;
}

// power
const Mat<int> pow(const Mat<int>& m1, size_t p){
    size_t row = m1.rows();
    size_t col = m1.cols();
    Mat<int> t;
    if (row == col){
        t = identity_mat(row);
        for (size_t i = 0; i < p; i++) {
            t *= m1;
        }
    }
    else{
        cerr << "mat's row must be equal to the its col." << std::endl;
    }
    return t;  
}

void mat_rowsum(Mat<int> mat, int rowsuml[]) {
    for (size_t i = 0; i < mat.rows(); i++) {
        int rowsum = 0;
        for (size_t j = 0; j < mat.cols(); j++) {
            rowsum += mat(i, j);
        }
        rowsuml[i] = rowsum;
    }
}

int mat_maxrowsum(Mat<int> mat) {
    int mrsum = 0;   
    for (size_t i = 0; i < mat.rows(); i++) {
        int rowsum = 0;
        for (size_t j = 0; j < mat.cols(); j++) {
            rowsum += mat(i, j);
        }
        if (mrsum < rowsum) {
            mrsum = rowsum;
        }
    }
    return(mrsum);
}

void mat_colsum(Mat<int> mat, int colsuml[]) {
    for (size_t j = 0; j < mat.cols(); j++) {
        int colsum = 0;
        for (size_t i = 0; i < mat.rows(); i++) {
            colsum += mat(i, j);
        }
        colsuml[j] = colsum;
    }
}

int mat_maxcolsum(Mat<int> mat) {
    int mcsum = 0;   
    for (size_t j = 0; j < mat.cols(); j++) {
        int colsum = 0;
        for (size_t i = 0; i < mat.rows(); i++) {
            colsum += mat(i, j);
        }
        if (mcsum < colsum) {
            mcsum = colsum;
        }
    }
    return(mcsum);
}

Mat<int> MAT_mod_operator(Mat<int> mat, int modn) {
    Mat<int> m(mat.rows(), mat.cols());
    for (int i = 0; i < mat.rows(); i++) {
        for (int j = 0; j < mat.cols(); j++) {
            m(i, j) = mat(i, j) % modn;
        }
    }
    return(m);
}

void insert_matrix_block_at_index(Mat<int> &expand_mat, int i, int j, Mat<int> matrix_block) {
    for (int k0 = 0; k0 < matrix_block.rows(); k0++) {
        for (int k1 = 0; k1 < matrix_block.cols(); k1++) {
            expand_mat(i * matrix_block.rows() + k0, j * matrix_block.cols() +  k1) = matrix_block(k0, k1);
        }
    }
}

Mat<int> F2n_value_2_F2_matrix(int F2n_value, Mat<int> generate_matrix) {
    Mat<int> iter = identity_mat(generate_matrix.rows());
    Mat<int> m = zero_mat(generate_matrix.rows(), generate_matrix.cols()); 
    for (int i = 0; i < generate_matrix.rows(); i++) {
        if (((F2n_value >> i) & 1) == 1) {
            m = MAT_mod_operator(m + iter, 2); 
        }
        iter = MAT_mod_operator(iter * generate_matrix, 2);
    } 
    return(m);
}



Mat<int> F2n_matrix_2_F2_matrix(Mat<int> matf2n, Mat<int> generate_matrix) {
    Mat<int> matf2(matf2n.rows() * generate_matrix.rows(), matf2n.cols() * generate_matrix.cols());
    for (int i = 0; i < matf2n.rows(); i++) {
        for (int j = 0; j < matf2n.cols(); j++) { 
            insert_matrix_block_at_index(matf2, i, j, F2n_value_2_F2_matrix(matf2n(i, j), generate_matrix));
        }
    } 
    return(matf2); 
}

Mat<int> subforms_of_matrix(Mat<int> m, vector<size_t> row_index, vector<size_t> col_index) {
    Mat<int> subm(row_index.size(), col_index.size());
    for (size_t i = 0; i < row_index.size(); i++) {
        for (size_t j = 0; j < col_index.size(); j++) {
            assert((row_index[i] < m.rows()) && (col_index[j] < m.cols()));
            subm(i, j) = m(row_index[i], col_index[j]);
        }
    } 
    return(subm);

}

vector<int> matrix_mul_vector(Mat<int> m, vector<int> v) {
    assert(m.cols() == v.size());
    vector<int> vm(m.rows());
    for (size_t i = 0; i < m.rows(); i++) {
        vm[i] = 0;
        for (size_t j = 0; j < m.cols(); j++) {
            vm[i] += m(i, j) * v[j]; 
        }
    }
    return(vm);
}

#endif
/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file DecisionTree.cpp
 * @author wangsirui01(com@baidu.com)
 * @date 2016/04/11 15:52:43
 * @brief 
 *  
 **/

#include<math.h>
#include<vector>

#define MAX_CLASS_NUMBER    8       //样本最多类别数
#define MAX_FEATURE_VALUE_NUMBER    32      //每种特征特征值数量

using namespace std;

struct Node{
    int feature_name;
    int type;
    Node *child[MAX_FEATURE_VALUE_NUMBER];
    Node():feature_name(-1),type(-1){
        for (int i = 0; i < MAX_FEATURE_VALUE_NUMBER; i++)
            child[i] = 0;        
    }

};

//-p*log2p,概率为0时，默认熵为0
double compute_entropy(double p)
{
    if(p)
        return -p * (log(p) / log(2));
    return 0;

}

double calculate_after_spliting_entropy(const vector<int> &feature_value, const vector<int> &class_value)
{
    int feature_value_count[MAX_FEATURE_VALUE_NUMBER] = {0}; //统计每个特征值的个数
    int feature_value_class_count[MAX_FEATURE_VALUE_NUMBER][MAX_CLASS_NUMBER] = {0};    //统计在每个特征值下标记的个数
    int sample_number = feature_value.size();
    for (int i = 0; i < sample_number; ++i)
    {
        feature_value_count[feature_value[i]] += 1;
        feature_value_class_count[feature_value[i]][class_value[i]] += 1;
    }
    double result = 0;
    for (int i = 0; i < MAX_FEATURE_VALUE_NUMBER; i++)
    {
        if (feature_value_count[feature_value] == 0)    //无样本值
            continue;
        double entropy = 0;
        for (int j = 0; j < MAX_CLASS_NUMBER; ++j)
        {
            double p = feature_value_class_count[i][j]/sample_number;
            entropy += compute_entropy(p);
        }
        result += (double)(feature_value_count[i]/sample_number)*entropy;   //某特征值在样本的概率*该特征值下的熵
    }
    return result;
}
/*
 *@input:当前数据标注class_value,当前剩余特征个数
 *@return:当前数据集类别
 */
int check_is_leaf(const vector<int> &class_value, int feature_number)
{
    int class_value_count[MAX_CLASS_NUMBER]; //统计标记数据的值
    int class_number = 0;   //不同标记值的个数
    for (size_t i = 0; i < class_value.size(); ++i)
    {
        if(class_value_count[class_value[i]] == 0)
            class_number++;
        class_value_count[class_value[i]]++;
    }
    int leaf_class = -1;
    int max_count = -1;
    for (size_t i = 0; i < MAX_CLASS_NUMBER; ++i)
    {
        if(class_value_count[i] > max_count)
        {
            max_count = class_value[i];
            leaf_class = i;
        }
    }
    if (class_number == 1 || feature_number == 0)
        return leaf_class;
    return -1;

}
/*
 * data:特征数据
 * feature_name:特征类别
 * class_value:分类类别
 * best_feature_name:最佳分裂特征
 * best_feature_name_pos:最佳分裂特征所处的列，供分裂函数使用
 */
void find_best_feature_name(vector<vector<int> > &data, vector<int> &class_value, vector<int> &featue_name, int &best_feature_name, int &best_feature_name_pos)
{
    vector<double> spliting_entropy;
    for (size_t j = 0; j < feature_name.size(); ++j)
    {
        vector<int> feature_value;
        for(size_t i = 0; i < data.size(); ++i)
        {
           feature_value.push_back(data[i][j]); 
        }
        double ret = calculate_after_spliting_entropy(&feature_value, class_value);
        spliting_entropy.push_back(ret);
    }
    best_feature_name = featue_name[0];
    best_feature_name_pos = 0;
    double min_entropy = spliting_entropy[0];
    for (size_t i = 0; i < spliting_entropy.size(); ++i)
    {
        if (min_entropy > spliting_entropy[i])
        {
            min_entropy = spliting_entropy[i];
            best_feature_name = feature_name[i];
            best_feature_name_pos = i;
        }
    }

}
/*
 * 原始数据:data,class_value,feature_name
 * 标注特征列:feature_pos
 * 特征值:feature_value
 * 生成数据:new...
 */

void get_data_by_feature_value(vector<int> &data, vector<int> &class_value, vector<int> &featue_name, int feature_pos, int featue_value, vector<int> &new_data, \
                                vector<int> &new_class_value, vector<int> &new_feature_name)
{
    for (size_t i = 0; i < feature_pos; ++i)
        new_feature_name.push_back(feature_name[i]);
    for (size_t j = feature_pos + 1; j < feature_name.size(); ++j)
        new_feature_name.push_back(feature_name[i]);

    for(size_t i = 0; i < data.size(); ++i)
    {
        vector<int> temp_vec;
        if (data[i][feature_pos] == feature_value)
        {
            for (size_t j = 0; j < feature_pos; ++j)
                temp_vec.push_back(data[i]);
            for (size_t j = feature_pos + 1; j < data[i].szie(); ++j)
                temp_vec.push_back(data[i]);
            new_data.push_back(temp_vec);
            new_class_value.push_back(class_value[i]);
        }
    }
}

Node* construct_desicion_tree(const vector<vector<int> > &data, const vector<int> &class_value, const vector<int> &feature_name, const vector<Node*> &tree)
{
    Node *node = new Node();
    tree.push_back(node);
    node->type = check_is_leaf(class_value, feature_name.size())
    if (node->type != -1)
    {
        return node;
    }
    int best_feature_name;
    int best_feature_name_pos;
    find_best_feature_name(&data, &class_value, &featue_name, &best_feature_name, &best_featue_name_pos);
    node->feature_name  = best_feature_name;
    set<int> value_set;     //存储最佳特征的全部特征值
    for (size_t i = 0; i < data.size(); ++i)
    {
        value_set.insert(data[i][best_featue_name_pos]);
    }
    for (set<int>::iterator it = value_set.begin(); it != value_set.end(); ++it)
    {
        vector<vector<int> > new_data;
        vector<int> new_class_value;
        vector<int> new_feature_name;
        get_data_by_feature_value(data, class_value, feature_name, best_feature_name_pos, i, new_data, new_class_value, new_feature_name);      
    
        node->child[*it] = construct_desicion_tree(new_data, new_class_value, new_feature_name, tree);
    }
    return node;
}
















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */

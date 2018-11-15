#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "RandomGenerator.hpp"
#include "LinearStringSearch.hpp"
#include <string>
#include <algorithm>
using namespace std;
template <class T,class Converter> string ArrayToString(vector<T> array,Converter convertToStr)
{
    string output;
    for(int i = 0; i < array.size();++i)
    {
        output += convertToStr(array[i]);
    }
    return output;
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbStart_clicked()
{
    string text = ui->pteInput10->toPlainText().toStdString();
    string output = "Input xml:\n";
    output +=text;
    output += "\nFind all indexes ,of the  end tags:\nNumber 1 ";
    vector<size_t> allEnds = LinearStringSearch::Find(text,">",output);
    auto sizeToStr = [](size_t element){
        return to_string(element+1) + " ";
     };
    output+=ArrayToString(allEnds,sizeToStr);

    vector<size_t> allBeginings = LinearStringSearch::Find(text,"<",output);
    output += "\nFind all indexes ,of the  begin tags:\nNumber 2 ";
    output+=ArrayToString(allBeginings,sizeToStr);

    vector<size_t> allBeginingsOfClose = LinearStringSearch::Find(text,"</",output);
    output += "\nFind all indexes ,of the  begin close-tags:\n Number 3 ";
    output+=ArrayToString(allBeginingsOfClose,sizeToStr);

    vector<size_t> allBeginingsOfOpen(allBeginings.size() - allBeginingsOfClose.size());
    set_difference(allBeginings.begin(),allBeginings.end(),allBeginingsOfClose.begin(),allBeginingsOfClose.end(),allBeginingsOfOpen.begin());
    output += "\nFind all indexes ,of the  begin open-tags:\nNumber 4 ";
    output+="Result: " + ArrayToString(allBeginingsOfOpen,sizeToStr) + '\n';
    output+='\n';
    size_t countOfAllTags = allBeginings.size();
    vector<bool> inspectorOfCorrectness(countOfAllTags);
    size_t  startIndexOfClose = allBeginingsOfClose.size();
    vector<bool> isUsedCloseTag(allBeginingsOfClose.size());
    for(size_t indexOfOpen = 0; indexOfOpen < allBeginingsOfOpen.size();++indexOfOpen)
    {
        for(size_t indexOfClose = startIndexOfClose;indexOfClose != 0;--indexOfClose)
        {
           bool isEqual = compareTwoSubstring(text,
                                allBeginingsOfOpen[indexOfOpen]+1,
                                allEnds[indexOfOpen],
                                allBeginingsOfClose[indexOfClose-1]+2,
                                allEnds[allBeginingsOfOpen.size()+indexOfClose-1],
                                output);
           if(isEqual)
           {
                startIndexOfClose = indexOfClose-1;
                isUsedCloseTag[indexOfClose - 1] = true;
                inspectorOfCorrectness[allBeginingsOfOpen.size() + indexOfClose - 1] = true;
                inspectorOfCorrectness[indexOfOpen] = true;
                output+= "Tags with indexes "+to_string(allBeginingsOfOpen[indexOfOpen]+1) +" and " +to_string(allBeginingsOfClose[indexOfClose-1]) + " has true position\n";
                break;
           }
        }
    }

    for(size_t indexOfOpen = 0; indexOfOpen < allBeginingsOfOpen.size();++indexOfOpen)
    {
        for(size_t indexOfClose = startIndexOfClose;indexOfClose != 0;--indexOfClose)
        {
           bool isEqual = compareTwoSubstring(text,
                                allBeginingsOfOpen[indexOfOpen]+1,
                                allEnds[indexOfOpen],
                                allBeginingsOfClose[indexOfClose-1]+2,
                                allEnds[allBeginingsOfOpen.size()+indexOfClose-1],
                                output);
           if(isEqual && !inspectorOfCorrectness[indexOfOpen] && !isUsedCloseTag[indexOfClose-1])
           {
                output+= "Open tag with index "+to_string(allBeginingsOfOpen[indexOfOpen]+1) + "has a pair, so it is true\n";
                isUsedCloseTag[indexOfClose -1 ] = true;
                inspectorOfCorrectness[indexOfOpen] = true;
                break;
           }
        }
    }
    bool isAllTrue = true;
    for(size_t i =0; i < inspectorOfCorrectness.size();++i)
    {
        if(!inspectorOfCorrectness[i])
        {
            output+="Tag with position " +  to_string(allBeginings[i]) + " is inappropriate\n";
            isAllTrue = false;
        }
    }
    if(isAllTrue)
    {
        output+="All is correct\n";
    }
    ui->pteOutput->setPlainText(QString::fromStdString(output));
}


void MainWindow::on_pbRandomize_clicked()
{
    RandomGenerator rand;
    size_t count = rand.generateCount(2,10);
    size_t openCount = (count/2) + rand.generateCount(0,1);
    string result;
    vector<string> namesOfTags(max(openCount,count-openCount));
    for(size_t i = 0; i < openCount;++i)
    {
        namesOfTags[i] = rand.generateName(static_cast<size_t>(1),
                                           static_cast<size_t>(5));
        result+="<" + namesOfTags[i] + '>';
    }
    result += rand.generateName(5,rand.generateCount(5,20));
    for(size_t i = 0; i < count-openCount;++i)
    {
        result+="</";
        result+=rand.generateCount(0,1)?(namesOfTags[namesOfTags.size()-i-1]):rand.generateName(namesOfTags[i].size() + 1);
        result+=">";
    }
    ui->pteInput10->setPlainText(QString::fromStdString(result));
}

 bool MainWindow::compareTwoSubstring(const std::string& text, size_t firstBegin,size_t firstEnd,size_t secondBegin, size_t secondEnd,string& output) const
 {
    string openTag,closeTag;
    for(size_t i = firstBegin; i< firstEnd;++i)
    {
        openTag += text[i];
    }
    for(size_t i = secondBegin; i< secondEnd;++i)
    {
        closeTag += text[i];
    }
    output+= "Comparing open tag <" + openTag +">" + " with close tag </" + closeTag+">\n";
    bool isEqual = true;
    if(firstEnd - firstBegin != secondEnd - secondBegin)
    {
        isEqual = false;
    }
    while(firstBegin < firstEnd && secondBegin < secondEnd && isEqual)
    {
        if(text[firstBegin] != text[secondBegin])
        {
            isEqual = false;
        }
        ++firstBegin;
        ++secondBegin;
    }
    if(isEqual)
    {
        output += "Tags are equal\n";
    }
    else
    {
        output += "Tags are not equal\n";
    }
    return isEqual;
 }

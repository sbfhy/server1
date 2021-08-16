#include "sudokuservice_stub.h"

using namespace RPC;

SudokuService_Stub::SudokuService_Stub()
{
}

void SudokuService_Stub::Solve_DoneCb(const ::sudoku::SudokuRequestPtr& request,
                                      const ::sudoku::SudokuResponsePtr& response) 
{
    LOG_DEBUG << request->ShortDebugString() << response->ShortDebugString();
}

void SudokuService_Stub::Solve_TimeOut(const ::sudoku::SudokuRequestPtr& request) 
{

}
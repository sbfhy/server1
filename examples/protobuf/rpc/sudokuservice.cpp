#include "sudokuservice.h"

using namespace RPC;

SudokuService::SudokuService() 
{

}

void SudokuService::Solve(const ::sudoku::SudokuRequestPtr& request,
                          const ::sudoku::SudokuResponsePtr& response)
{
    {LOG_DEBUG << request->ShortDebugString();}
    if (!response) return ;
    response->set_solved(true);
    response->set_checkerboard("1234567");
}

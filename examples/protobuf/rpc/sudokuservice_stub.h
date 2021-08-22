#pragma once

#include "message/test/sudoku.pb.h"
#include "src/base/mgr/create_service.h"

namespace RPC {
    
class SudokuService_Stub : public CreateService<SudokuService_Stub>
                         , public sudoku::SudokuService_Stub
{
public:
    SudokuService_Stub();

    virtual void Solve_DoneCb(const ::sudoku::SudokuRequestPtr& request,
                              const ::sudoku::SudokuResponsePtr& response) override;

    virtual void Solve_TimeOut(const ::sudoku::SudokuRequestPtr& request) override;
};

}   // namespace RPC


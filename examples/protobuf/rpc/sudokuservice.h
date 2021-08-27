#pragma once

#include "message/test/sudoku.pb.h"
#include "src/base/mgr/create_dynamic.h"

namespace RPC {

class SudokuService : public CreateDynamicEmpty<SudokuService>
                    , public sudoku::SudokuService
{
public:
    SudokuService();
  
    virtual void Solve(const ::sudoku::SudokuRequestPtr& request,
                       const ::sudoku::SudokuResponsePtr& response) override;
};

}   // namespace RPC


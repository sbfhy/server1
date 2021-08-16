#pragma once

#include "message/test/sudoku.pb.h"
#include "src/base/mgr/create_service.h"

namespace RPC {

class SudokuService : public CreateService<SudokuService>
                    , public sudoku::SudokuService
{
public:
    SudokuService();
  
    virtual void Solve(const ::sudoku::SudokuRequestPtr& request,
                       const ::sudoku::SudokuResponsePtr& response) override;
};

}   // namespace RPC


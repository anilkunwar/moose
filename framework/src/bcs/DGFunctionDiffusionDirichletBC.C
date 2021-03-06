/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "DGFunctionDiffusionDirichletBC.h"
#include "Function.h"

#include "libmesh/numeric_vector.h"

#include <cmath>

template<>
InputParameters validParams<DGFunctionDiffusionDirichletBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addParam<Real>("value", 0.0, "The value the variable should have on the boundary");
  params.addRequiredParam<FunctionName>("function", "The forcing function.");
  params.addRequiredParam<Real>("epsilon", "Epsilon");
  params.addRequiredParam<Real>("sigma", "Sigma");

  return params;
}

DGFunctionDiffusionDirichletBC::DGFunctionDiffusionDirichletBC(const std::string & name, InputParameters parameters) :
    IntegratedBC(name, parameters),
    _func(getFunction("function")),
    _epsilon(getParam<Real>("epsilon")),
    _sigma(getParam<Real>("sigma"))
{
}

Real
DGFunctionDiffusionDirichletBC::computeQpResidual()
{
  const unsigned int elem_b_order = static_cast<unsigned int> (_var.getOrder());
  const double h_elem = _current_elem->volume()/_current_side_elem->volume() * 1./std::pow(elem_b_order, 2.);

  Real fn = _func.value(_t, _q_point[_qp]);
  Real r = 0;
  r -= (_grad_u[_qp] * _normals[_qp] * _test[_i][_qp]);
  r += _epsilon * (_u[_qp] - fn) * _grad_test[_i][_qp] * _normals[_qp];
  r += _sigma/h_elem * (_u[_qp] - fn) * _test[_i][_qp];

  return r;
}

Real
DGFunctionDiffusionDirichletBC::computeQpJacobian()
{
  const unsigned int elem_b_order = static_cast<unsigned int> (_var.getOrder());
  const double h_elem = _current_elem->volume()/_current_side_elem->volume() * 1./std::pow(elem_b_order, 2.);

  Real r = 0;
  r -= (_grad_phi[_j][_qp] * _normals[_qp] * _test[_i][_qp]);
  r += _epsilon * _phi[_j][_qp] * _grad_test[_i][_qp] * _normals[_qp];
  r += _sigma/h_elem * _phi[_j][_qp] * _test[_i][_qp];

  return r;
}

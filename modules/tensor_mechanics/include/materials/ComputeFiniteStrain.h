#ifndef COMPUTEFINITESTRAIN_H
#define COMPUTEFINITESTRAIN_H

#include "ComputeStrainBase.h"

/**
 * ComputeFiniteStrain defines a the strain increment and rotation increment, for finite strains.
 */
class ComputeFiniteStrain : public ComputeStrainBase
{
public:
  ComputeFiniteStrain(const std:: string & name, InputParameters parameters);

protected:
  virtual void initQpStatefulProperties();
  virtual void computeProperties();
  virtual void computeQpStrain(const RankTwoTensor & Fhat);

  MaterialProperty<RankTwoTensor> & _strain_rate;
  MaterialProperty<RankTwoTensor> & _strain_increment;
  MaterialProperty<RankTwoTensor> & _total_strain_old;
  MaterialProperty<RankTwoTensor> & _rotation_increment;

  MaterialProperty<RankTwoTensor> & _deformation_gradient;

  const MaterialProperty<RankTwoTensor> & _stress_free_strain_increment;
  VariableValue & _T_old;

};

#endif //COMPUTEFINITESTRAIN_H

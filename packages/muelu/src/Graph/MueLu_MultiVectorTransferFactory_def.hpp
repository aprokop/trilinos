#ifndef MUELU_MULTIVECTORTRANSFER_FACTORY_DEF_HPP
#define MUELU_MULTIVECTORTRANSFER_FACTORY_DEF_HPP

#include "MueLu_MultiVectorTransferFactory_decl.hpp"
#include "Xpetra_MultiVectorFactory.hpp"

// #include <Xpetra_Operator.hpp>

#include "MueLu_Level.hpp"
// #include "MueLu_Monitor.hpp"

namespace MueLu {

  // ----------------------------------------------------------------------------------------
  // Constructor
  // ----------------------------------------------------------------------------------------

  template <class Scalar, class LocalOrdinal, class GlobalOrdinal, class Node, class LocalMatOps>
  MultiVectorTransferFactory<Scalar, LocalOrdinal, GlobalOrdinal, Node, LocalMatOps>::MultiVectorTransferFactory(
    std::string const & vectorName,
    std::string const & restrictionName,
    RCP<const FactoryBase> const &restrictionFact)
    : vectorName_(vectorName),
      restrictionName_(restrictionName),
      restrictionFact_(restrictionFact)
  { }

  // ----------------------------------------------------------------------------------------
  // Destructor
  // ----------------------------------------------------------------------------------------

  template <class Scalar, class LocalOrdinal, class GlobalOrdinal, class Node, class LocalMatOps>
  MultiVectorTransferFactory<Scalar, LocalOrdinal, GlobalOrdinal, Node, LocalMatOps>::~MultiVectorTransferFactory() {}

  // ----------------------------------------------------------------------------------------
  // DeclareInput
  // ----------------------------------------------------------------------------------------

  template <class Scalar, class LocalOrdinal, class GlobalOrdinal, class Node, class LocalMatOps>
  void MultiVectorTransferFactory<Scalar, LocalOrdinal, GlobalOrdinal, Node, LocalMatOps>::DeclareInput(Level &fineLevel, Level &coarseLevel) const {
    fineLevel.DeclareInput(vectorName_,MueLu::NoFactory::get(),this);
    coarseLevel.DeclareInput(restrictionName_,restrictionFact_.get(),this);
  }

  // ----------------------------------------------------------------------------------------
  // Build
  // ----------------------------------------------------------------------------------------

  template <class Scalar, class LocalOrdinal, class GlobalOrdinal, class Node, class LocalMatOps>
  void MultiVectorTransferFactory<Scalar, LocalOrdinal, GlobalOrdinal, Node, LocalMatOps>::Build(Level & fineLevel, Level &coarseLevel) const {

/*
    //FIXME debugging output
    RCP<Teuchos::FancyOStream> fos = Teuchos::fancyOStream(Teuchos::rcpFromRef(std::cout));
    fos->setOutputToRootOnly(-1);
    *fos << "inside MueLu::MultiVectorTransferFactory::Build()\n" << std::endl;
    *fos << "printing fineLevel " << fineLevel.GetLevelID() << std::endl;
    fineLevel.print(*fos,Teuchos::VERB_EXTREME);
    *fos << "************************************************************************************************************"
         << std::endl;
    *fos << "printing coarseLevel " << coarseLevel.GetLevelID() << std::endl;
    coarseLevel.print(*fos,Teuchos::VERB_EXTREME);
    //FIXME end of debugging output
*/
    
    TEUCHOS_TEST_FOR_EXCEPTION(!fineLevel.IsAvailable(vectorName_,MueLu::NoFactory::get()), Exceptions::RuntimeError,
                        "MueLu::MultiVectorTransferFactory::Build(): vector '" + vectorName_ + "' is not available.");

    RCP<MultiVector> vector  = fineLevel.Get<RCP<MultiVector> >(vectorName_,MueLu::NoFactory::get());
    RCP<Operator> transferOp = coarseLevel.Get<RCP<Operator> >(restrictionName_,restrictionFact_.get());

    RCP<MultiVector> result = MultiVectorFactory::Build(transferOp->getDomainMap(),1);
    transferOp->apply(*vector,*result,Teuchos::TRANS);
    coarseLevel.Set<RCP<MultiVector> >(vectorName_,result,this);
    //transferOp->describe(*fos,Teuchos::VERB_EXTREME);
    
  } //Build

  //TODO do we need methods to get name of MultiVector and or Transfer Operator?

} // namespace MueLu

#define MUELU_MULTIVECTORTRANSFER_FACTORY_SHORT
#endif // MUELU_MULTIVECTORTRANSFER_FACTORY_DEF_HPP

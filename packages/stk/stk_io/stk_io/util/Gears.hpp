/*------------------------------------------------------------------------*/
/*                 Copyright 2010 Sandia Corporation.                     */
/*  Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive   */
/*  license for use of this work by or on behalf of the U.S. Government.  */
/*  Export of this program may require a license from the                 */
/*  United States Government.                                             */
/*------------------------------------------------------------------------*/

#ifndef stk_io_util_Gears_hpp
#define stk_io_util_Gears_hpp

#include <vector>

#include <stk_util/parallel/Parallel.hpp>
#include <stk_mesh/base/Types.hpp>
#include <stk_mesh/base/Field.hpp>
#ifndef SKIP_DEPRECATED_STK_MESH_TOPOLOGY_HELPERS
#include <stk_mesh/fem/FieldDeclarations.hpp>
#endif /* SKIP_DEPRECATED_STK_MESH_TOPOLOGY_HELPERS */
#include <stk_mesh/base/MetaData.hpp>
#include <stk_mesh/fem/DefaultFEM.hpp>
#include <stk_mesh/fem/TopologyDimensions.hpp>
#include <stk_mesh/fem/CoordinateSystems.hpp>

namespace stk {
  namespace mesh {
    class MetaData;
    class BulkData;
  }
  
  namespace io {
    namespace util {

      struct GearFields {

	enum { SpatialDimension = 3 };

	typedef stk::mesh::Field<double,stk::mesh::Cartesian>            CartesianField ;
	typedef stk::mesh::Field<double,stk::mesh::Cylindrical>          CylindricalField ;

	CylindricalField & gear_coord ;
	CartesianField   & model_coord ;

	GearFields( stk::mesh::MetaData & S );

      private:
	GearFields();
	GearFields( const GearFields & );
	GearFields & operator = ( const GearFields & );
      };

      class Gear {
      public:
	Gear( stk::mesh::MetaData & S ,
	      const std::string & name ,
	      const GearFields & gear_fields ,
	      const double   center[] ,
	      const double   rad_min ,
	      const double   rad_max ,
	      const size_t   rad_num ,
	      const double   z_min ,
	      const double   z_max ,
	      const size_t   z_num ,
	      const size_t   angle_num ,
	      const int      turn_direction );

	void mesh( stk::mesh::BulkData &M );
	void turn( double turn_angle ) const ;

	stk::mesh::MetaData & m_mesh_meta_data ;
#ifdef SKIP_DEPRECATED_STK_MESH_TOPOLOGY_HELPERS
        stk::mesh::DefaultFEM m_topo_data;
#endif /* SKIP_DEPRECATED_STK_MESH_TOPOLOGY_HELPERS */
	stk::mesh::BulkData * m_mesh ;
	stk::mesh::Part & m_gear ;
	stk::mesh::Part & m_surf ;
	const GearFields::CylindricalField  & m_gear_coord ;
	const GearFields::CartesianField    & m_model_coord ;

      private:

	Gear( const Gear & );
	Gear & operator = ( const Gear & );

	double m_center[3] ;
	double m_z_min ;
	double m_z_max ;
	double m_z_inc ;
	double m_rad_min ;
	double m_rad_max ;
	double m_rad_inc ;
	double m_ang_inc ;
	size_t m_rad_num ;
	size_t m_z_num ;
	size_t m_angle_num ;
	int    m_turn_dir ;

	stk::mesh::Entity &create_node( const std::vector<stk::mesh::Part*> &parts ,
					stk::mesh::EntityId node_id_base ,
					size_t iz ,
					size_t ir ,
					size_t ia ) const ;
      };

    }
  }
}

#endif


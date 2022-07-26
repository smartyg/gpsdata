#ifndef _GPSDATA_TRAITS_COMMON_
#define _GPSDATA_TRAITS_COMMON_

#if __cplusplus >= 202002L
#define SAME_TYPE(type) std::same_as<type>
#define CONVERTIBLE_TYPE(type) std::convertible_to<type>
#define DERIVED_FROM(type1, type2) std::derived_from<type1, type2>
#define CONCEPT_RETURN
#else
#define SAME_TYPE(type) type
#define CONVERTIBLE_TYPE(type) type
#define DERIVED_FROM(type1, type2) std::is_base_of<type1, type2>
#define CONCEPT_RETURN bool
#endif

#endif /* _GPSDATA_TRAITS_COMMON_ */
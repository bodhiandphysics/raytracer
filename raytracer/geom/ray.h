namespace geom {

	struct ray {

		vec3 origin;
		vec3 direction; // should always be normalized;

		ray(const vec3 &anorigin, const vec3 &unnormalized);
		ray reflect(const vec3 &point,const vec3 &normal);
		bool refract(const vec3& point, const vec3& normal, const nicefp& inref_index, const nicefp& outref_index, ray& result);

	};
}
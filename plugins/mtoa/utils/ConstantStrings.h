// Copyright 2019 Autodesk, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
/// @file constant_strings.h
///
/// File holding shared, constant definitions of AtString instances.
///
/// Defining EXPAND_MTOA_STRINGS before including constantStrings.h will not only
/// declare but also define the AtString instances.
#pragma once

#include <ai_string.h>

#include <maya/MString.h>

namespace str {

#ifdef EXPAND_MTOA_STRINGS
#define ASTR(x)                 \
    extern const AtString x;    \
    const AtString x(#x);       \
    extern const MString m_##x; \
    const MString m_##x(#x)
#define ASTR2(x, y)             \
    extern const AtString x;    \
    const AtString x(y);        \
    extern const MString m_##x; \
    const MString m_##x(y)
#else
#define ASTR(x)              \
    extern const AtString x; \
    extern const MString m_##x
#define ASTR2(x, y)          \
    extern const AtString x; \
    extern const MString m_##x
#endif


/** In this section, we use ASTR, which will allow us to get static AtString and MString.
*   For example, ASTR(animatable) will allow us to get :
*   AtString s = str::animatable;
*   MString ms = str::m_animatable; 
**/
ASTR(__mtoa_baking_cam);
ASTR(animatable);
ASTR(AA_samples);
ASTR(aov_shader);
ASTR(alembic);
ASTR(angle);
ASTR(append);
ASTR(aov_indirect);
ASTR(aspect_ratio);
ASTR(autocrop);
ASTR(average);
ASTR(base);
ASTR(base_color);
ASTR(bounds);
ASTR(camera);
ASTR(cameraUvBaker);
ASTR(cast_volumetric_shadows);
ASTR(catclark);
ASTR(code);
ASTR(color_gradient_input_bias);
ASTR(color_gradient_type);
ASTR(color_space);
ASTR(color_manager);
ASTR(colorManagement);
ASTR(colors);
ASTR(cone_angle);
ASTR(coordinates);
ASTR(cosine_power);
ASTR(dcc_name);
ASTR(density);
ASTR(density_method);
ASTR(desc);
ASTR(display_driver);
ASTR(driver);
ASTR(driver_exr);
ASTR(emission);
ASTR(emission_color);
ASTR(enable_dependency_graph);
ASTR(extend_edges);
ASTR(exposure);
ASTR(filename);
ASTR(filter);
ASTR(filter_type);
ASTR(format);
ASTR(fps);
ASTR(frame);
ASTR(front);
ASTR(fov);
ASTR(fuel_method);
ASTR(gamma);
ASTR(gaussian_filter);
ASTR(geometry_matrix);
ASTR(GI_diffuse_samples);
ASTR(GI_specular_samples);
ASTR(GI_sss_samples);
ASTR(GI_transmission_samples);
ASTR(GI_volume_samples);
ASTR(horizontal_fov);
ASTR(image);
ASTR(imager);
ASTR(incandescence_gradient_input_bias);
ASTR(incandescence_gradient_type);
ASTR(index);
ASTR(input1);
ASTR(input2);
ASTR(inputs);
ASTR(intensity);
ASTR(is_target);
ASTR(Kd);
ASTR(Kd_color);
ASTR(kDependNode);
ASTR(kLocatorNode);
ASTR(Kt);
ASTR(label);
ASTR(layer_selection);
ASTR(layers);
ASTR(lens_radius);
ASTR(lens_shift);
ASTR(lens_tilt_angle);
ASTR(light);
ASTR(linear);
ASTR(mask);
ASTR(material_surface);
ASTR(material_displacement);
ASTR(matrix);
ASTR(max);
ASTR(merge);
ASTR(mesh);
ASTR(meters_per_unit);
ASTR(min);
ASTR(mode);
ASTR(motion_start);
ASTR(motion_end);
ASTR(name);
ASTR(normalize);
ASTR(nsides);
ASTR(near_clip);
ASTR(object_path);
ASTR(objectpath);
ASTR(offset);
ASTR(opacity);
ASTR(opacity_gradient_input_bias);
ASTR(opacity_gradient_type);
ASTR(opaque);
ASTR(outputMode);
ASTR(outputs);
ASTR(overrides);
ASTR(penumbra_angle);
ASTR(persp_camera);
ASTR(polymesh);
ASTR(portal);
ASTR(portal_mode);
ASTR(procedural);
ASTR(procedural_searchpath);
ASTR(progressive);
ASTR(projective);
ASTR(radial_distortion);
ASTR(radial_distortion_type);
ASTR(radius);
ASTR(region_max_x);
ASTR(region_max_y);
ASTR(region_min_x);
ASTR(region_min_y);
ASTR(renderSession);
ASTR(resolution);
ASTR(roundness);
ASTR(shader);
ASTR(shape);
ASTR(skydome_light);
ASTR(sidedness);
ASTR(shadow_color);
ASTR(skip_license_check);
ASTR(soft_edge);
ASTR(softmax);
ASTR(softmin);
ASTR(specular_transmission);
ASTR(spread);
ASTR(subdiv_adaptive_error);
ASTR(subdiv_adaptive_metric);
ASTR(subdiv_adaptive_space);
ASTR(subdiv_iterations);
ASTR(subdiv_smooth_derivs);
ASTR(subdiv_type);
ASTR(subdiv_uv_smoothing);
ASTR(subtype);
ASTR(swatch);
ASTR(temperature_method);
ASTR(tiled);
ASTR(total_progress);
ASTR(transmission);
ASTR(u_offset);
ASTR(u_scale);
ASTR(usd);
ASTR(uv_camera);
ASTR(uv_remap);
ASTR(uv_set);
ASTR(uvidxs);
ASTR(uvlist);
ASTR(v_offset);
ASTR(v_scale);
ASTR(velocity);
ASTR(velocity_method);
ASTR(vertical_fov);
ASTR(vertices);
ASTR(vidxs);
ASTR(view);
ASTR(visibility);
ASTR(vlist);
ASTR(width);
ASTR(xres);
ASTR(yres);



/** In this section, use ASTR2 to specify a given key and string. 
*   We need this when specific characters are in the string
**/
ASTR2(aov_type, "aov.type");
ASTR2(_default, "default");
ASTR2(constant_BOOL, "constant BOOL");
ASTR2(light__filter, "light/filter");
ASTR2(maya_attr_prefix, "maya.attr_prefix");
ASTR2(maya_channelbox, "maya.channelbox");
ASTR2(maya_classification, "maya.classification");
ASTR2(maya_connectable, "maya.connectable");
ASTR2(maya_counterpart, "maya.counterpart");
ASTR2(maya_counterpart_id, "maya.counterpart_id");
ASTR2(maya_drawdb, "maya.drawdb");
ASTR2(maya_keyable, "maya.keyable");
ASTR2(maya_hide, "maya.hide");
ASTR2(maya_id, "maya.id");
ASTR2(maya_isdefault, "maya.isdefault");
ASTR2(maya_lights, "maya.lights");
ASTR2(maya_name, "maya.name");
ASTR2(maya_output, "maya.output");
ASTR2(maya_output_name, "maya.output_name");
ASTR2(maya_output_shortname, "maya.output_shortname");
ASTR2(maya_procedural, "maya.procedural");
ASTR2(maya_shortname, "maya.shortname");
ASTR2(maya_translator, "maya.translator");
ASTR2(maya_type, "maya.type");
ASTR2(maya_usedAsFilename, "maya.usedAsFilename");
ASTR2(_operator, "operator");


#undef ASTR
#undef ASTR2

} // namespace str


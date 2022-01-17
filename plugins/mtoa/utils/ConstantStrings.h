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
ASTR(a);
ASTR(alpha_multiply);
ASTR(animatable);
ASTR(AA_samples);
ASTR(AA_sample_clamp);
ASTR(AA_sample_clamp_affects_aovs);
ASTR(AA_seed);
ASTR(alembic);
ASTR(angle);
ASTR(aperture_aspect_ratio);
ASTR(aperture_blade_curvature);
ASTR(aperture_blades);
ASTR(aperture_rotation);
ASTR(aperture_size);
ASTR(append);
ASTR(atmosphere);
ASTR(aov);
ASTR(aov_indirect);
ASTR(aov_input);
ASTR(aov_name);
ASTR(aov_shader);
ASTR(aov_shaders);
ASTR(aspect_ratio);
ASTR(assignment);
ASTR(autocrop);
ASTR(average);
ASTR(b);
ASTR(background);
ASTR(background_visibility);
ASTR(base);
ASTR(base_color);
ASTR(binary);
ASTR(bitangent);
ASTR(bounds);
ASTR(box);
ASTR(bucket_scanning);
ASTR(bucket_size);
ASTR(camera);
ASTR(cameraUvBaker);
ASTR(cast_shadows);
ASTR(cast_volumetric_shadows);
ASTR(catclark);
ASTR(code);
ASTR(color_gradient_input_bias);
ASTR(color_gradient_type);
ASTR(color_space);
ASTR(color_manager);
ASTR(colorManagement);
ASTR(color);
ASTR(colors);
ASTR(cone_angle);
ASTR(cosine_power);
ASTR(coordinates);
ASTR(coverage);
ASTR(CPU);
ASTR(dcc_name);
ASTR(deepexr);
ASTR(density);
ASTR(density_method);
ASTR(desc);
ASTR(diffuse);
ASTR(direction);
ASTR(display_driver);
ASTR(distant_light);
ASTR(driver);
ASTR(driver_exr);
ASTR(emission);
ASTR(emission_color);
ASTR(enable);
ASTR(enable_dependency_graph);
ASTR(error);
ASTR(extend_edges);
ASTR(exposure);
ASTR(exr);
ASTR(far_clip);
ASTR(filename);
ASTR(filter);
ASTR(filter_type);
ASTR(filter_weights);
ASTR(filtermap);
ASTR(filters);
ASTR(focus_distance);
ASTR(format);
ASTR(fps);
ASTR(frame);
ASTR(front);
ASTR(fov);
ASTR(fuel_method);
ASTR(g);
ASTR(gamma);
ASTR(gaussian_filter);
ASTR(geometry);
ASTR(geometry_matrix);
ASTR(GI_diffuse_depth);
ASTR(GI_diffuse_samples);
ASTR(GI_specular_depth);
ASTR(GI_specular_samples);
ASTR(GI_sss_samples);
ASTR(GI_total_depth);
ASTR(GI_transmission_depth);
ASTR(GI_transmission_samples);
ASTR(GI_volume_samples);
ASTR(ginstance);
ASTR(GPU);
ASTR(gpu_default_min_memory_MB);
ASTR(gpu_default_names);
ASTR(horizontal_fov);
ASTR(id);
ASTR(ignore_displacement);
ASTR(ignore_list);
ASTR(ignore_missing_textures);
ASTR(image);
ASTR(imager);
ASTR(incandescence_gradient_input_bias);
ASTR(incandescence_gradient_type);
ASTR(index);
ASTR(indirect);
ASTR(inherit_xform);
ASTR(input);
ASTR(input1);
ASTR(input2);
ASTR(inputs);
ASTR(intensity);
ASTR(is_swatch);
ASTR(is_target);
ASTR(jpeg);
ASTR(jpg);
ASTR(Kd);
ASTR(Kd_color);
ASTR(kDependNode);
ASTR(kLocatorNode);
ASTR(Kt);
ASTR(label);
ASTR(layer_enable);
ASTR(layer_exposure);
ASTR(layer_intensity);
ASTR(layer_name);
ASTR(layer_selection);
ASTR(layer_solo);
ASTR(layer_tint);
ASTR(layers);
ASTR(lens_radius);
ASTR(lens_shift);
ASTR(lens_tilt_angle);
ASTR(light);
ASTR(light_group);
ASTR(light_path_expressions);
ASTR(linear);
ASTR(linkable);
ASTR(look_at);
ASTR(luminance);
ASTR(mask);
ASTR(material_surface);
ASTR(material_displacement);
ASTR(materialview_display);
ASTR(materialview_display1);
ASTR(materialview_filter1);
ASTR(matrix);
ASTR(matte);
ASTR(max);
ASTR(max_bounces);
ASTR(maya_full_name);
ASTR(merge);
ASTR(mesh);
ASTR(mesh_light);
ASTR(meters_per_unit);
ASTR(min);
ASTR(mode);
ASTR(motion_start);
ASTR(motion_end);
ASTR(mtrlViewDummyShader);
ASTR(name);
ASTR(nidxs);
ASTR(nlist);
ASTR(node);
ASTR(normalize);
ASTR(nsides);
ASTR(near_clip);
ASTR(object_path);
ASTR(objectpath);
ASTR(off);
ASTR(offset);
ASTR(opacity);
ASTR(opacity_gradient_input_bias);
ASTR(opacity_gradient_type);
ASTR(opaque);
ASTR(open_procs);
ASTR(output_name);
ASTR(outputMode);
ASTR(outputs);
ASTR(overrides);
ASTR(override_nodes);
ASTR(passthrough);
ASTR(penumbra_angle);
ASTR(periodic);
ASTR(persp_camera);
ASTR(pin_threads);
ASTR(pixel_aspect_ratio);
ASTR(points);
ASTR(polymesh);
ASTR(portal);
ASTR(portal_mode);
ASTR(position);
ASTR(procedural);
ASTR(procedural_searchpath);
ASTR(progressive);
ASTR(projective);
ASTR(quad_light);
ASTR(r);
ASTR(radial_distortion);
ASTR(radial_distortion_type);
ASTR(radius);
ASTR(raw_driver);
ASTR(receive_shadows);
ASTR(reference_time);
ASTR(region_max_x);
ASTR(region_max_y);
ASTR(region_min_x);
ASTR(region_min_y);
ASTR(render_device);
ASTR(render_device_fallback);
ASTR(render_layer);
ASTR(renderSession);
ASTR(renderview_display);
ASTR(resolution);
ASTR(rolling_shutter);
ASTR(rolling_shutter_duration);
ASTR(rotate);
ASTR(roundness);
ASTR(samples);
ASTR(scalar_mode);
ASTR(scale_frame);
ASTR(scene);
ASTR(screen_window_max);
ASTR(screen_window_min);
ASTR(self_shadows);
ASTR(set_parameter);
ASTR(shader);
ASTR(shadow_color);
ASTR(shadow_density);
ASTR(shadow_group);
ASTR(shape);
ASTR(shutter_curve);
ASTR(shutter_end);
ASTR(shutter_start);
ASTR(shutter_type);
ASTR(sidedness);
ASTR(single_layer_driver);
ASTR(skip_license_check);
ASTR(sky);
ASTR(skydome_light);
ASTR(smoothing);
ASTR(soft_edge);
ASTR(softmax);
ASTR(softmin);
ASTR(specular);
ASTR(specular_transmission);
ASTR(sphere);
ASTR(spot_light);
ASTR(spread);
ASTR(sss);
ASTR(sss_setname);
ASTR(standard_surface);
ASTR(subdiv_adaptive_error);
ASTR(subdiv_adaptive_metric);
ASTR(subdiv_adaptive_space);
ASTR(subdiv_dicing_camera);
ASTR(subdiv_iterations);
ASTR(subdiv_smooth_derivs);
ASTR(subdiv_type);
ASTR(subdiv_uv_smoothing);
ASTR(subtype);
ASTR(swatch);
ASTR(swatch_renderview_display);
ASTR(swatch_renderview_filter);
ASTR(tangent);
ASTR(temperature_method);
ASTR(texture_automip);
ASTR(texture_autotile);
ASTR(texture_max_sharpen);
ASTR(texture_per_file_stats);
ASTR(texture_searchpath);
ASTR(texture_use_existing_tx);
ASTR(threads);
ASTR(tiled);
ASTR(toon_id);
ASTR(total_progress);
ASTR(trace_sets);
ASTR(transmission);
ASTR(translate_frame);
ASTR(u_offset);
ASTR(u_scale);
ASTR(usd);
ASTR(use_light_group);
ASTR(use_shadow_group);
ASTR(user);
ASTR(utility);
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
ASTR(volume);
ASTR(volume_samples);
ASTR(width);
ASTR(wrap_frame_u);
ASTR(wrap_frame_v);
ASTR(xres);
ASTR(yres);



/** In this section, use ASTR2 to specify a given key and string. 
*   We need this when specific characters are in the string
**/
ASTR2(aov_type, "aov.type");
ASTR2(_default, "default");
ASTR2(constant_ARRAY_NODE,"constant ARRAY NODE");
ASTR2(constant_BOOL, "constant BOOL");
ASTR2(constant_STRING, "constant STRING");
ASTR2(light__filter, "light/filter");
ASTR2(maya_attr_prefix, "maya.attr_prefix");
ASTR2(maya_attrs, "maya.attrs");
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
ASTR2(_namespace, "namespace");
ASTR2(_operator, "operator");
ASTR2(varying_VECTOR, "varying VECTOR");

#undef ASTR
#undef ASTR2

} // namespace str


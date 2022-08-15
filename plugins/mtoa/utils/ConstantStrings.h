
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
ASTR(AA_samples);
ASTR(AA_sample_clamp);
ASTR(AA_sample_clamp_affects_aovs);
ASTR(AA_seed);
ASTR(alembic);
ASTR(alpha_is_luminance);
ASTR(alpha_multiply);
ASTR(amplitude);
ASTR(animatable);
ASTR(angle);
ASTR(aov);
ASTR(aov_indirect);
ASTR(aov_input);
ASTR(aov_name);
ASTR(aov_shader);
ASTR(aov_shaders);
ASTR(aperture_aspect_ratio);
ASTR(aperture_blade_curvature);
ASTR(aperture_blades);
ASTR(aperture_rotation);
ASTR(aperture_size);
ASTR(append);
ASTR(aspect);
ASTR(aspect_ratio);
ASTR(assignment);
ASTR(attribute);
ASTR(atmosphere);
ASTR(auto_instancing);
ASTR(autobump_visibility);
ASTR(autocrop);
ASTR(average);
ASTR(b);
ASTR(background);
ASTR(background_visibility);
ASTR(ball);
ASTR(base);
ASTR(base_color);
ASTR(basis);
ASTR(bias);
ASTR(binary);
ASTR(bitangent);
ASTR(bitangentidxs);
ASTR(bounds);
ASTR(box);
ASTR(BTref);
ASTR(BTrefidxs);
ASTR(bucket_scanning);
ASTR(bucket_size);
ASTR(camera);
ASTR(camera_projection);
ASTR(cameraUvBaker);
ASTR(cast_shadows);
ASTR(cast_volumetric_shadows);
ASTR(catclark);
ASTR(circular);
ASTR(clamp);
ASTR(coat);
ASTR(coat_color);
ASTR(coat_IOR);
ASTR(coat_roughness);
ASTR(code);
ASTR(color);
ASTR(color_gradient_input_bias);
ASTR(color_gradient_type);
ASTR(color_manager);
ASTR(color_space);
ASTR(color_to_signed);
ASTR(color1);
ASTR(color2);
ASTR(colorManagement);
ASTR(colors);
ASTR(compress);
ASTR(cone_angle);
ASTR(contrast_pivot);
ASTR(cosine_power);
ASTR(coord_space);
ASTR(coordinates);
ASTR(coverage);
ASTR(CPU);
ASTR(crease_idxs);
ASTR(crease_sharpness);
ASTR(cubic);
ASTR(custom);
ASTR(curves_only);
ASTR(dcc_name);
ASTR(deepexr);
ASTR(density);
ASTR(density_method);
ASTR(desc);
ASTR(diagonal);
ASTR(diffuse);
ASTR(direction);
ASTR(disable_ray_extents);
ASTR(disk);
ASTR(disp_autobump);
ASTR(disp_height);
ASTR(disp_map);
ASTR(disp_padding);
ASTR(disp_zero_value);
ASTR(display_driver);
ASTR(distant_light);
ASTR(divide);
ASTR(dPdu);
ASTR(dPdv);
ASTR(driver);
ASTR(driver_exr);
ASTR(emission);
ASTR(emission_color);
ASTR(enable);
ASTR(enable_deformation_blur);
ASTR(enable_dependency_graph);
ASTR(enable8);
ASTR(error);
ASTR(exclude_xform);
ASTR(extend_edges);
ASTR(exponent);
ASTR(exposure);
ASTR(exr);
ASTR(falloff_exponent);
ASTR(far_clip);
ASTR(field);
ASTR(field_channel);
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
ASTR(frequency);
ASTR(from);
ASTR(front);
ASTR(fov);
ASTR(fuel_method);
ASTR(function);
ASTR(g);
ASTR(gain);
ASTR(gamma);
ASTR(gaussian_filter);
ASTR(geometry);
ASTR(geometry_matrix);
ASTR(Gradient);
ASTR(Grid);
ASTR(grids);
ASTR(ground_albedo);
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
ASTR(HSV);
ASTR(id);
ASTR(ignore_displacement);
ASTR(ignore_list);
ASTR(ignore_missing_textures);
ASTR(image);
ASTR(imager);
ASTR(incandescence_gradient_input_bias);
ASTR(incandescence_gradient_type);
ASTR(incandescencePP);
ASTR(index);
ASTR(indirect);
ASTR(indirect_diffuse);
ASTR(inherit_xform);
ASTR(input);
ASTR(input0);
ASTR(input1);
ASTR(input2);
ASTR(input8);
ASTR(inputs);
ASTR(intensity);
ASTR(interpolation);
ASTR(invert_normals);
ASTR(invert_z);
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
ASTR(lambert);
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
ASTR(lifespanPP);
ASTR(light);
ASTR(light_group);
ASTR(light_path_expressions);
ASTR(lights);
ASTR(linear);
ASTR(linkable);
ASTR(look_at);
ASTR(luminance);
ASTR(make_instance);
ASTR(mask);
ASTR(material_surface);
ASTR(material_displacement);
ASTR(material_volume);
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
ASTR(min_pixel_width);
ASTR(mirror);
ASTR(mirror_u);
ASTR(mirror_v);
ASTR(missing);
ASTR(missing_texture_color);
ASTR(mode);
ASTR(motion_start);
ASTR(motion_end);
ASTR(motion_vector_scale);
ASTR(mtoa_fluid_data);
ASTR(mtrlViewDummyShader);
ASTR(multiply);
ASTR(N);
ASTR(name);
ASTR(near_clip);
ASTR(Nf);
ASTR(nidxs);
ASTR(nlist);
ASTR(node);
ASTR(none);
ASTR(normal);
ASTR(normalize);
ASTR(Nref);
ASTR(Nrefidxs);
ASTR(num_points);
ASTR(numInputs);
ASTR(nsides);
ASTR(object);
ASTR(object_path);
ASTR(objectpath);
ASTR(off);
ASTR(offset);
ASTR(opacity);
ASTR(opacity_gradient_input_bias);
ASTR(opacity_gradient_type);
ASTR(opacityPP);
ASTR(opaque);
ASTR(open_procs);
ASTR(operation);
ASTR(operation8);
ASTR(order);
ASTR(orientations);
ASTR(osl);
ASTR(output_max);
ASTR(output_min);
ASTR(output_name);
ASTR(outputMode);
ASTR(outputs);
ASTR(overrides);
ASTR(override_nodes);
ASTR(P);
ASTR(particleId);
ASTR(particleMultiIndex);
ASTR(passthrough);
ASTR(penumbra_angle);
ASTR(periodic);
ASTR(persp_camera);
ASTR(phase_func);
ASTR(planar);
ASTR(pin_threads);
ASTR(pixel_aspect_ratio);
ASTR(Po);
ASTR(point);
ASTR(points);
ASTR(polygon_holes);
ASTR(polymesh);
ASTR(portal);
ASTR(portal_mode);
ASTR(position);
ASTR(Pref);
ASTR(procedural);
ASTR(procedural_searchpath);
ASTR(progressive);
ASTR(projection_type);
ASTR(projective);
ASTR(pull_user_params);
ASTR(quad);
ASTR(quad_light);
ASTR(r);
ASTR(radial);
ASTR(radial_distortion);
ASTR(radial_distortion_type);
ASTR(radius);
ASTR(radius_attribute);
ASTR(radius_default);
ASTR(radius_scale);
ASTR(radiusPP);
ASTR(ray_switch_rgba);
ASTR(raw_driver);
ASTR(Rd);
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
ASTR(repeat);
ASTR(resolution);
ASTR(RGB);
ASTR(rgbPP);
ASTR(rim_light);
ASTR(rolling_shutter);
ASTR(rolling_shutter_duration);
ASTR(rotate);
ASTR(rotate_frame);
ASTR(roughness);
ASTR(roundness);
ASTR(samples);
ASTR(scalar);
ASTR(scalar_mode);
ASTR(scale_frame);
ASTR(scale);
ASTR(scene);
ASTR(screen_window_max);
ASTR(screen_window_min);
ASTR(self_shadows);
ASTR(set_parameter);
ASTR(shade_mode);
ASTR(shader);
ASTR(shadow_color);
ASTR(shadow_density);
ASTR(shadow_group);
ASTR(shape);
ASTR(shapes);
ASTR(shidxs);
ASTR(shutter_curve);
ASTR(shutter_end);
ASTR(shutter_start);
ASTR(shutter_type);
ASTR(sidedness);
ASTR(sin);
ASTR(single_layer_driver);
ASTR(skip_license_check);
ASTR(sky);
ASTR(sky_tint);
ASTR(skydome_light);
ASTR(smooth_step);
ASTR(smoothing);
ASTR(soffset);
ASTR(soft_edge);
ASTR(softmax);
ASTR(softmin);
ASTR(solver);
ASTR(specular);
ASTR(specular_anisotropy);
ASTR(specular_color);
ASTR(specular_IOR);
ASTR(specular_rotation);
ASTR(specular_roughness);
ASTR(specular_transmission);
ASTR(specular2_tint);
ASTR(sphere);
ASTR(spherical);
ASTR(spot_light);
ASTR(spread);
ASTR(sscale);
ASTR(sss);
ASTR(sss_setname);
ASTR(standard_surface);
ASTR(step_scale);
ASTR(step_size);
ASTR(strength);
ASTR(subdiv_adaptive_error);
ASTR(subdiv_adaptive_metric);
ASTR(subdiv_adaptive_space);
ASTR(subdiv_dicing_camera);
ASTR(subdiv_frustum_ignore);
ASTR(subdiv_iterations);
ASTR(subdiv_smooth_derivs);
ASTR(subdiv_type);
ASTR(subdiv_uv_smoothing);
ASTR(subtype);
ASTR(sun_direction);
ASTR(sun_tint);
ASTR(swatch);
ASTR(swatch_renderview_display);
ASTR(swatch_renderview_filter);
ASTR(swrap);
ASTR(tangent);
ASTR(tangent_space);
ASTR(tangentidxs);
ASTR(temperature_gradient);
ASTR(temperature_method);
ASTR(test);
ASTR(texture_automip);
ASTR(texture_autotile);
ASTR(texture_max_sharpen);
ASTR(texture_per_file_stats);
ASTR(texture_searchpath);
ASTR(texture_use_existing_tx);
ASTR(threads);
ASTR(threshold);
ASTR(tiled);
ASTR(time);
ASTR(to);
ASTR(toffset);
ASTR(toon_id);
ASTR(total_progress);
ASTR(trace_sets);
ASTR(transmission);
ASTR(transmission_tint);
ASTR(transparency);
ASTR(translate_frame);
ASTR(Tref);
ASTR(Trefidxs);
ASTR(triplanar);
ASTR(tscale);
ASTR(twrap);
ASTR(type);
ASTR(u);
ASTR(u_angle);
ASTR(u_frequency);
ASTR(u_offset);
ASTR(u_scale);
ASTR(usd);
ASTR(use_implicit_uvs);
ASTR(use_instance_cache);
ASTR(use_light_group);
ASTR(use_shadow_group);
ASTR(user);
ASTR(utility);
ASTR(universe);
ASTR(uv);
ASTR(uv_camera);
ASTR(uv_remap);
ASTR(uv_projection);
ASTR(uv_set);
ASTR(uvs);
ASTR(uvset);
ASTR(uvidxs);
ASTR(uvlist);
ASTR(v);
ASTR(v_offset);
ASTR(v_scale);
ASTR(value);
ASTR(variable);
ASTR(vector);
ASTR(velocity);
ASTR(velocity_method);
ASTR(vertical_fov);
ASTR(vertices);
ASTR(v_angle);
ASTR(v_frequency);
ASTR(velocity_fps);
ASTR(velocity_grids);
ASTR(velocity_outlier_threshold);
ASTR(velocity_scale);
ASTR(vidxs);
ASTR(view);
ASTR(visibility);
ASTR(visibility_ignore);
ASTR(vlist);
ASTR(volume);
ASTR(volume_padding);
ASTR(volume_samples);
ASTR(volume_texture);
ASTR(width);
ASTR(world);
ASTR(wrap_frame_color);
ASTR(wrap_frame_u);
ASTR(wrap_frame_v);
ASTR(wrap_uvs);
ASTR(x);
ASTR(X);
ASTR(xres);
ASTR(XZY);
ASTR(y);
ASTR(Y);
ASTR(yres);
ASTR(YXZ);
ASTR(Z);
ASTR(zres);
ASTR(outValue);
ASTR(outTransparency);



/** In this section, use ASTR2 to specify a given key and string. 
*   We need this when specific characters are in the string
**/
ASTR2(aov_type, "aov.type");
ASTR2(_default, "default");
ASTR2(catmull_rom, "catmull-rom");
ASTR2(constant_ARRAY_INT, "constant ARRAY INT");
ASTR2(constant_ARRAY_FLOAT, "constant ARRAY FLOAT");
ASTR2(constant_ARRAY_NODE,"constant ARRAY NODE");
ASTR2(constant_ARRAY_RGB,"constant ARRAY RGB");
ASTR2(constant_ARRAY_VECTOR,"constant ARRAY VECTOR");
ASTR2(constant_BOOL, "constant BOOL");
ASTR2(constant_FLOAT, "constant FLOAT");
ASTR2(constant_INT, "constant INT");
ASTR2(constant_NODE, "constant NODE");
ASTR2(constant_RGB, "constant RGB");
ASTR2(constant_STRING, "constant STRING");
ASTR2(constant_VECTOR, "constant VECTOR");
ASTR2(curves__basis, "curves:basis");
ASTR2(indexed_UINT, "indexed UINT");
ASTR2(indexed_RGBA, "indexed RGBA");
ASTR2(indexed_VECTOR, "indexed VECTOR");
ASTR2(indexed_VECTOR2, "indexed VECTOR2");
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
ASTR2(uniform_ARRAY_RGB, "uniform ARRAY RGB");
ASTR2(uniform_INT, "uniform INT");
ASTR2(uniform_FLOAT, "uniform FLOAT");
ASTR2(uniform_RGB, "uniform RGB");
ASTR2(uniform_VECTOR, "uniform VECTOR");
ASTR2(varying_POINT, "varying POINT");
ASTR2(varying_RGB, "varying RGB");
ASTR2(varying_RGBA, "varying RGBA");
ASTR2(varying_VECTOR, "varying VECTOR");

#undef ASTR
#undef ASTR2

} // namespace str


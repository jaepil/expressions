//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

// This code is based on an example of Painless script from Elasticsearch.

package org.expressions.examples.smile;


is_in_circle = (pos_x, pos_y, circle_x, circle_y, radius) => {
    dist_from_center = (((circle_x - pos_x) ** 2.0)
                        + ((circle_y - pos_y) ** 2.0)) ** 0.5;
    return dist_from_center <= radius;
};

is_on_circle = (pos_x, pos_y, circle_x, circle_y,
                radius, thickness, squash_y) => {
    dist_from_center = ((circle_x - pos_x) ** 2
                        + ((circle_y - pos_y) / squash_y) ** 2) ** 0.5;
    return dist_from_center >= radius - thickness
            and dist_from_center <= radius + thickness;
};

draw = () => {
    result = "";
    char_count = 0;

    width = 31;
    height = 31;
    half_width = width / 2;
    half_height = height / 2;

    stroke_width = 0.6;

    head_size = 13;
    head_squash_y = 0.78;
    eye_position_x = 10;
    eye_position_y = 12;
    eye_size = 1;
    mouth_size = 15;
    mouth_position_x = width / 2;
    mouth_position_y = 5;
    mouth_offset_y = 11;

    for (y = 0; y < height; y += 1) {
        for (x = 0; x < width; x += 1) {
            is_head = is_on_circle(x, y, half_width, half_height, head_size,
                                stroke_width, head_squash_y);
            is_left_eye = is_in_circle(x, y, eye_position_x, eye_position_y,
                                    eye_size);
            is_right_eye = is_in_circle(x, y, (width - eye_position_x - 1),
                                        eye_position_y, eye_size);
            is_mouth = is_on_circle(x, y, mouth_position_x, mouth_position_y,
                                    mouth_size, stroke_width, 1)
                        and y > mouth_position_y + mouth_offset_y;

            if (is_left_eye or is_right_eye or is_mouth or is_head) {
                result += "*";
            } else {
                result += ".";
            }

            result += " ";

            char_count += 1;
            if (char_count % width == 0) {
                result += "\n";
            }
        }
    }

    return result;
}

return draw();

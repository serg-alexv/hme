#!/usr/bin/env python3
"""Render a deterministic preview sprite from the supplied ASCII FBX dragon.

This is an offline asset-build tool. The Windows runtime consumes the resulting
PNG; it does not parse FBX or require Python on the investor machine.
"""

from __future__ import annotations

import argparse
import math
import re
from pathlib import Path

import numpy as np
from PIL import Image, ImageDraw


def _array_between(text: str, start: str, end: str, dtype: type) -> np.ndarray:
    match = re.search(start + r"\s*(.*?)\n\s*" + end, text, re.S)
    if match is None:
        raise ValueError(f"FBX section not found: {start}")
    payload = re.sub(r"^\s*a:\s*", "", match.group(1).strip())
    return np.fromstring(payload.replace("\n", ""), sep=",", dtype=dtype)


def load_mesh(path: Path) -> tuple[np.ndarray, list[tuple[int, int, int]]]:
    text = path.read_text(encoding="utf-8", errors="ignore")
    vertices = _array_between(
        text, r"Vertices:", r"PolygonVertexIndex:", float
    ).reshape((-1, 3))
    raw_indices = _array_between(
        text, r"PolygonVertexIndex:", r"GeometryVersion:", int
    )

    polygons: list[list[int]] = []
    current: list[int] = []
    for value in raw_indices:
        index = int(value)
        if index < 0:
            current.append(-index - 1)
            polygons.append(current)
            current = []
        else:
            current.append(index)

    triangles: list[tuple[int, int, int]] = []
    for polygon in polygons:
        if len(polygon) < 3:
            continue
        for offset in range(1, len(polygon) - 1):
            triangles.append((polygon[0], polygon[offset], polygon[offset + 1]))
    return vertices, triangles


def deform(vertices: np.ndarray, wing: float, tail: float) -> np.ndarray:
    """Apply a bounded presentation-only pose to the actual dragon mesh."""
    posed = vertices.copy()

    # The asset is front-facing on X/Y. Outer X contains both wings. Rotate
    # around shoulder anchors while preserving the central body.
    for sign in (-1.0, 1.0):
        mask = (posed[:, 0] * sign > 18.0) & (posed[:, 1] > 12.0)
        anchor = np.array([sign * 16.0, 38.0])
        points = posed[mask, :2] - anchor
        angle = sign * wing
        cosine, sine = math.cos(angle), math.sin(angle)
        rotation = np.array([[cosine, -sine], [sine, cosine]])
        posed[mask, :2] = points @ rotation.T + anchor

    # The long lower/rear section bends smoothly instead of moving as a block.
    tail_mask = posed[:, 1] < 10.0
    strength = np.clip((10.0 - posed[tail_mask, 1]) / 40.0, 0.0, 1.0)
    posed[tail_mask, 0] += np.sin(strength * math.pi) * tail * 16.0
    return posed


def render(
    vertices: np.ndarray,
    triangles: list[tuple[int, int, int]],
    output: Path,
    size: int,
    wing: float,
    tail: float,
    pitch_degrees: float,
) -> None:
    vertices = deform(vertices, wing, tail)
    pitch = math.radians(pitch_degrees)
    cosine, sine = math.cos(pitch), math.sin(pitch)
    pitch_rotation = np.array(
        [[1.0, 0.0, 0.0], [0.0, cosine, -sine], [0.0, sine, cosine]]
    )
    vertices = vertices @ pitch_rotation.T
    xy = vertices[:, :2]
    minimum = xy.min(axis=0)
    maximum = xy.max(axis=0)
    extent = maximum - minimum
    scale = (size * 0.86) / max(float(extent[0]), float(extent[1]))
    center = (minimum + maximum) * 0.5
    projected = np.empty_like(xy)
    projected[:, 0] = (xy[:, 0] - center[0]) * scale + size * 0.5
    projected[:, 1] = size * 0.53 - (xy[:, 1] - center[1]) * scale

    image = Image.new("RGBA", (size, size), (0, 0, 0, 0))
    draw = ImageDraw.Draw(image, "RGBA")
    light = np.array([-0.35, 0.5, 0.79])
    light /= np.linalg.norm(light)

    ordered: list[tuple[float, tuple[int, int, int], float]] = []
    for triangle in triangles:
        a, b, c = vertices[list(triangle)]
        normal = np.cross(b - a, c - a)
        norm = float(np.linalg.norm(normal))
        if norm < 1.0e-8:
            continue
        normal /= norm
        shade = 0.22 + 0.78 * abs(float(np.dot(normal, light)))
        ordered.append((float((a[2] + b[2] + c[2]) / 3.0), triangle, shade))

    ordered.sort(key=lambda item: item[0])
    for _, triangle, shade in ordered:
        points = [tuple(projected[index]) for index in triangle]
        red = int(18 + 25 * shade)
        green = int(50 + 118 * shade)
        blue = int(62 + 128 * shade)
        draw.polygon(points, fill=(red, green, blue, 245))

    bounds = image.getbbox()
    if bounds is not None:
        cropped = image.crop(bounds)
        maximum = int(size * 0.94)
        factor = min(maximum / cropped.width, maximum / cropped.height)
        resized = cropped.resize(
            (max(1, int(cropped.width * factor)), max(1, int(cropped.height * factor))),
            Image.Resampling.LANCZOS,
        )
        image = Image.new("RGBA", (size, size), (0, 0, 0, 0))
        image.alpha_composite(
            resized,
            ((size - resized.width) // 2, (size - resized.height) // 2),
        )

    output.parent.mkdir(parents=True, exist_ok=True)
    image.save(output, optimize=True)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("fbx", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--size", type=int, default=1024)
    parser.add_argument("--wing", type=float, default=0.0)
    parser.add_argument("--tail", type=float, default=0.0)
    parser.add_argument("--pitch", type=float, default=0.0)
    args = parser.parse_args()
    vertices, triangles = load_mesh(args.fbx)
    render(
        vertices,
        triangles,
        args.output,
        args.size,
        args.wing,
        args.tail,
        args.pitch,
    )
    print(
        f"rendered {len(vertices)} vertices / {len(triangles)} triangles -> {args.output}"
    )


if __name__ == "__main__":
    main()
